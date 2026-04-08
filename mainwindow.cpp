#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QButtonGroup"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "customlistdelegate.h"
#include "customtabledelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QApplication>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QButtonGroup *btns = new QButtonGroup();
    btns->addButton(ui->btn1,0);
    btns->addButton(ui->btn2,1);
    btns->addButton(ui->btn3,2);
    btns->addButton(ui->btn4,3);
    btns->addButton(ui->btn5,4);
    connect(btns,&QButtonGroup::idClicked,ui->stackedWidget,&QStackedWidget::setCurrentIndex);
    btns->button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    m_strList<<"北京"<<"上海"<<"成都"<<"武汉"<<"南京";
    m_model_list = new QStringListModel(this);
    m_model_list->setStringList(m_strList);
    ui->listView->setModel(m_model_list);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    ui->listView->setItemDelegate(new CustomListDelegate(this));

    m_model_table = new QStandardItemModel(4, 6, this);
    m_model_table->setHorizontalHeaderLabels({
        "姓名", "城市", "年龄", "状态", "备注", "图标"
    });

    // 初始化测试数据
    for(int row=0; row<4; row++){
        m_model_table->setItem(row, 0, new QStandardItem("用户_" + QString::number(row)));
        m_model_table->setItem(row, 1, new QStandardItem("北京"));
        m_model_table->setItem(row, 2, new QStandardItem("25"));
        // m_model_table->setItem(row, 3, new QStandardItem(Qt::Checked)); // 勾选状态
        QStandardItem *item = new QStandardItem();
        item->setData(false, Qt::EditRole);  // 存状态
        item->setText("");                  // 不显示任何文字
        m_model_table->setItem(row, 3, item);

        m_model_table->setItem(row, 4, new QStandardItem("测试备注"));
        m_model_table->setItem(row, 5, new QStandardItem("图片"));
    }

    m_model_table_select = new QItemSelectionModel(m_model_table,this);
    ui->tableView->setModel(m_model_table);
    ui->tableView->setSelectionModel(m_model_table_select);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    // ui->tableView->setItemDelegate(new CustomTableDelegate(this));
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    // ==============================================
    // 核心：给每一行、每一列 直接设置永久控件
    // ==============================================
    int rowCount = m_model_table->rowCount();
    for (int row = 0; row < rowCount; row++)
    {
        // ========== 第0列：永久 QLineEdit（姓名） ==========
        QLineEdit *edit = new QLineEdit(this);
        edit->setText(m_model_table->index(row, 0).data().toString());
        ui->tableView->setIndexWidget(m_model_table->index(row, 0), edit);
        connect(edit, &QLineEdit::textChanged, this, [=](const QString &text){
            m_model_table->setData(m_model_table->index(row, 0), text);
        });

        // ========== 第1列：永久 QComboBox（城市） ==========
        QComboBox *cb = new QComboBox(this);
        cb->addItems({"北京","上海","广州","深圳","成都"});
        cb->setCurrentText(m_model_table->index(row, 1).data().toString());
        ui->tableView->setIndexWidget(m_model_table->index(row, 1), cb);
        connect(cb, &QComboBox::currentTextChanged, this, [=](const QString &text){
            m_model_table->setData(m_model_table->index(row, 1), text);
        });

        // ========== 第2列：永久 QSpinBox（年龄） ==========
        QSpinBox *spin = new QSpinBox(this);
        spin->setRange(0, 100);
        spin->setValue(m_model_table->index(row, 2).data().toInt());
        ui->tableView->setIndexWidget(m_model_table->index(row, 2), spin);
        connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int val){
            m_model_table->setData(m_model_table->index(row, 2), val);
        });

        // ========== 第3列：永久 QCheckBox（状态） ==========
        // QCheckBox *ck = new QCheckBox(this);
        // ck->setChecked(m_model_table->index(row, 3).data().toBool());
        // m_model_table->setData(m_model_table->index(row, 3), "", Qt::DisplayRole);
        // ui->tableView->setIndexWidget(m_model_table->index(row, 3), ck);
        // connect(ck, &QCheckBox::clicked, this, [=](bool checked){
        //     m_model_table->setData(m_model_table->index(row, 3), checked);
        // });
        // ========== 第3列：永久 QCheckBox（状态） ==========
        QCheckBox *ck = new QCheckBox(this);
        ck->setText(""); // 关键！复选框本身不要文字！

        auto idx = m_model_table->index(row, 3);
        bool checked = idx.data(Qt::EditRole).toBool();
        ck->setChecked(checked);

        ui->tableView->setIndexWidget(idx, ck);

        connect(ck, &QCheckBox::clicked, this, [=](bool checked){
            m_model_table->setData(idx, checked, Qt::EditRole);
        });

        // ========== 第4列：永久 QLineEdit（备注） ==========
        QLineEdit *edit2 = new QLineEdit(this);
        edit2->setText(m_model_table->index(row, 4).data().toString());
        ui->tableView->setIndexWidget(m_model_table->index(row, 4), edit2);
        connect(edit2, &QLineEdit::textChanged, this, [=](const QString &text){
            m_model_table->setData(m_model_table->index(row, 4), text);
        });
    }

    // 基础使用，不自定义
    model_jichu = new QStandardItemModel(4,4,this);
    model_jichu->setHorizontalHeaderLabels({"姓名", "年龄","性别" ,"城市"});

    for (int row = 0; row < 4; ++row) {
        model_jichu->setItem(row,0,new QStandardItem("张三"));
        model_jichu->setItem(row,1,new QStandardItem("18"));
        model_jichu->setItem(row,2,new QStandardItem("男"));
        model_jichu->setItem(row,3,new QStandardItem("武汉"));
    }
    model_jichu_select = new QItemSelectionModel(model_jichu,this);
    ui->jichu->setModel(model_jichu);
    ui->jichu->setSelectionModel(model_jichu_select);
    ui->jichu->horizontalHeader()->setStretchLastSection(true);
    // ========== 选中行为配置 ==========
    // 1. 整行选中
    ui->jichu->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 2. 整列选中（二选一，不要同时开）
    // ui->jichu->setSelectionBehavior(QAbstractItemView::SelectColumns);

    // 选择模式
    ui->jichu->setSelectionMode(QAbstractItemView::SingleSelection);   // 单选
    // ui->jichu->setSelectionMode(QAbstractItemView::MultiSelection);  // 点击多选
    // ui->jichu->setSelectionMode(QAbstractItemView::ExtendedSelection); // Ctrl/Shift 多选

    // 可选：让选中行高亮更明显
    ui->jichu->setAlternatingRowColors(true);
    // 禁止编辑
    ui->jichu->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 表头点击也能整行/列选中
    ui->jichu->horizontalHeader()->setSectionsClickable(true);
    ui->jichu->verticalHeader()->setSectionsClickable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_model_list->setStringList(m_strList);
}


void MainWindow::on_pushButton_2_clicked()
{
    m_model_list->removeRows(0,m_model_list->rowCount());
}


void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked){
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    }else{
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    m_model_list->insertRow(m_model_list->rowCount());
    QModelIndex index = m_model_list->index(m_model_list->rowCount()-1);
    m_model_list->setData(index,"new item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}


void MainWindow::on_pushButton_8_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    m_model_list->insertRow(index.row());
    m_model_list->setData(index,"new item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}


void MainWindow::on_pushButton_7_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    int row = index.row();
    if (row <= 0) return;
    m_model_list->moveRow(QModelIndex(), row, QModelIndex(), row-1);
}


void MainWindow::on_pushButton_9_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    int row = index.row();
    if (row >= m_model_list->rowCount()-1) return;
    m_model_list->moveRow(QModelIndex(), row, QModelIndex(), row+2);
}

void MainWindow::on_pushButton_10_clicked(bool checked)
{
    if (checked){
        m_model_list->sort(Qt::AscendingOrder);
    }else {
        m_model_list->sort(Qt::DescendingOrder);
    }
}


void MainWindow::on_pushButton_11_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    if (!index.isValid()) {
        return;
    }
    int row = index.row();
    m_model_list->removeRow(row);
}


void MainWindow::on_pushButton_13_clicked()
{
    ui->plainTextEdit->clear();
    QStringList resultList = m_model_list->stringList();
    for (int var = 0; var < resultList.size(); ++var) {
        ui->plainTextEdit->appendPlainText(resultList.at(var));

    }
}


void MainWindow::on_pushButton_12_clicked()
{
    ui->plainTextEdit->clear();
}


void MainWindow::do_currentChange_table(const QModelIndex &current,const QModelIndex &pre){

}

void MainWindow::on_pushButton_4_clicked()
{
    // 清空文本框
    ui->plainTextEdit_2->clear();

    // 获取行数、列数
    int rowCount = m_model_table->rowCount();
    int colCount = m_model_table->columnCount();

    QString text;

    // 拼接表头
    QStringList headers;
    for (int c = 0; c < colCount; ++c) {
        headers << m_model_table->horizontalHeaderItem(c)->text();
    }
    text += headers.join("\t") + "\n";
    text += "------------------------------------------------\n";

    // 拼接每一行数据
    for (int r = 0; r < rowCount; ++r) {
        QStringList rowData;
        for (int c = 0; c < colCount; ++c) {
            QModelIndex index = m_model_table->index(r, c);
            rowData << index.data().toString();
        }
        text += rowData.join("\t") + "\n";
    }

    // 显示到 plainTextEdit_2
    ui->plainTextEdit_2->setPlainText(text);
}


void MainWindow::on_pushButton_3_clicked()
{
    // 1. 清空现有所有数据
    m_model_table->clear();

    // 2. 重新设置行列数
    m_model_table->setRowCount(4);
    m_model_table->setColumnCount(6);

    // 3. 恢复表头
    m_model_table->setHorizontalHeaderLabels({
        "姓名", "城市", "年龄", "状态", "备注", "图标"
    });

    // 4. 恢复初始测试数据
    for(int row = 0; row < 4; row++){
        m_model_table->setItem(row, 0, new QStandardItem("用户_" + QString::number(row)));
        m_model_table->setItem(row, 1, new QStandardItem("北京"));
        m_model_table->setItem(row, 2, new QStandardItem("25"));
        m_model_table->setItem(row, 3, new QStandardItem(Qt::Checked));  // 复选框正常勾选
        m_model_table->setItem(row, 4, new QStandardItem("测试备注"));
        m_model_table->setItem(row, 5, new QStandardItem("图片"));
    }

}

