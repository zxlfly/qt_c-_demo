#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QButtonGroup"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "customlistdelegate.h"
#include "mvcdetailcomboboxdelegate.h"
#include "mvcdetailqspinboxdelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QApplication>
#include <QListView>
#include "customtabledelegate.h"
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
    btns->addButton(ui->btn6,5);
    connect(btns,&QButtonGroup::idClicked,ui->stackedWidget,&QStackedWidget::setCurrentIndex);
    btns->button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
    // list示例
    m_strList<<"北京"<<"上海"<<"成都"<<"武汉"<<"南京";
    m_model_list = new QStringListModel(this);
    m_model_list->setStringList(m_strList);
    ui->listView->setModel(m_model_list);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    ui->listView->setItemDelegate(new CustomListDelegate(this));

    // ************ 基础使用，不自定义 ************
    model_jichu = new QStandardItemModel(4,4,this);
    model_jichu->setHorizontalHeaderLabels({"姓名", "年龄","性别" ,"城市"});

    for (int row = 0; row < 4; ++row) {
        model_jichu->setItem(row,0,new QStandardItem("基础使用"));
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
    // ui->jichu->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 表头点击也能整行/列选中
    ui->jichu->horizontalHeader()->setSectionsClickable(true);
    ui->jichu->verticalHeader()->setSectionsClickable(true);

    // ************ mvc完整基础示例 ************
    model_mvc = new QStandardItemModel(4,5,this);  // 修正：改为5列
    model_mvc->setHorizontalHeaderLabels({"完整基础示例", "列1", "列2", "列3", "列4"});  // 添加表头
    model_mvc_select = new QItemSelectionModel(model_mvc,this);
    for (int row = 0; row < 4; ++row) {
        model_mvc->setItem(row,0,new QStandardItem(QString::number(row)));  // 列0: 行号
        model_mvc->setItem(row,1,new QStandardItem(QString::number(10 + row)));  // 列1: SpinBox (整数)
        model_mvc->setItem(row,2,new QStandardItem(QString::number(20 + row)));  // 列2: SpinBox (整数)
        model_mvc->setItem(row,3,new QStandardItem(QString::number(30 + row)));  // 列3: SpinBox (整数)
        model_mvc->setItem(row,4,new QStandardItem("A"));  // 列4: ComboBox (字符串)
    };
    mvcDetailQSpinBoxDelegate = new MvcDetailQSpinBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(1,mvcDetailQSpinBoxDelegate);
    ui->tableView->setItemDelegateForColumn(2,mvcDetailQSpinBoxDelegate);
    ui->tableView->setItemDelegateForColumn(3,mvcDetailQSpinBoxDelegate);

    mvcDetailComboBoxDelegate = new MvcDetailComboBoxDelegate(this);
    QStringList strList;
    strList<<"A"<<"B"<<"C"<<"D";
    mvcDetailComboBoxDelegate->setMvcDetailComboBoxDelegateList(strList,false);
    ui->tableView->setItemDelegateForColumn(4,mvcDetailComboBoxDelegate);  // 修正：使用正确的委托
    ui->tableView->setModel(model_mvc);
    ui->tableView->setSelectionModel(model_mvc_select);
    
    // 可选：配置 tableView 的显示和行为
    // ui->tableView->horizontalHeader()->setStretchLastSection(true);
    // ui->tableView->setAlternatingRowColors(true);
    // ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // ************ 不用委托，默认绘制渲染和编辑使用同一个控件 ************
    model_buyong = new QStandardItemModel(4, 4, this);
    model_buyong->setHorizontalHeaderLabels({"姓名", "年龄", "性别", "备注（不用委托，默认绘制渲染和编辑使用同一个控件）"});
    model_buyong_select = new QItemSelectionModel(model_buyong, this);

    ui->buyong->setModel(model_buyong);
    ui->buyong->setSelectionModel(model_buyong_select);

    // 禁用默认委托编辑
    ui->buyong->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 为每个单元格创建持久化 Widget 并连接双向同步
    for (int row = 0; row < 4; ++row) {
        // 列0：QLineEdit（姓名）
        QLineEdit *leName = new QLineEdit(ui->buyong);
        leName->setText("张三");
        ui->buyong->setIndexWidget(model_buyong->index(row, 0), leName);
        connect(leName, &QLineEdit::editingFinished, [this, row, leName]() {
            model_buyong->setData(model_buyong->index(row, 0), leName->text());
        });

        // 列1：QSpinBox（年龄）
        QSpinBox *sbAge = new QSpinBox(ui->buyong);
        sbAge->setRange(0, 150);
        sbAge->setValue(18 + row);
        ui->buyong->setIndexWidget(model_buyong->index(row, 1), sbAge);
        connect(sbAge, QOverload<int>::of(&QSpinBox::valueChanged), [this, row](int val) {
            model_buyong->setData(model_buyong->index(row, 1), val);
        });

        // 列2：QComboBox（性别）
        QComboBox *cbGender = new QComboBox(ui->buyong);
        cbGender->addItems({"男", "女"});
        cbGender->setCurrentIndex(0);
        ui->buyong->setIndexWidget(model_buyong->index(row, 2), cbGender);
        connect(cbGender, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, row, cbGender](int) {
            model_buyong->setData(model_buyong->index(row, 2), cbGender->currentText());
        });

        // 列3：QLineEdit（备注）
        QLineEdit *leNote = new QLineEdit(ui->buyong);
        leNote->setText("备注" + QString::number(row));
        ui->buyong->setIndexWidget(model_buyong->index(row, 3), leNote);
        connect(leNote, &QLineEdit::editingFinished, [this, row, leNote]() {
            model_buyong->setData(model_buyong->index(row, 3), leNote->text());
        });
    }

    // model → widget 反向同步
    connect(model_buyong, &QStandardItemModel::dataChanged,
            [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
                // topLeft 变化区域的左上角单元格
                //bottomRight变化区域的右下角单元格
                //roles哪些数据角色发生了变化（如 DisplayRole、EditRole 等）
                // 只处理 DisplayRole 的数据变化，避免循环触发
                if (!roles.isEmpty() && !roles.contains(Qt::DisplayRole)) return;
                // 遍历变化区域内的所有单元格
                for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
                    for (int col = topLeft.column(); col <= bottomRight.column(); ++col) {
                        // 获取该单元格对应的 widget
                        QWidget *w = ui->buyong->indexWidget(model_buyong->index(row, col));
                        if (!w) continue;
                        // 从 model 读取最新数据
                        QVariant data = model_buyong->data(model_buyong->index(row, col));
                        // 根据控件类型，把 model 数据设置到 widget  qobject_cast 尝试把 QWidget 转成 QLineEdit,转换成功说明是这个类型,比较当前值和新值，不同才设置（避免光标跳动）
                        if (QLineEdit *le = qobject_cast<QLineEdit*>(w)) {
                            if (le->text() != data.toString()) le->setText(data.toString());
                        } else if (QSpinBox *sb = qobject_cast<QSpinBox*>(w)) {
                            if (sb->value() != data.toInt()) sb->setValue(data.toInt());
                        } else if (QComboBox *cb = qobject_cast<QComboBox*>(w)) {
                            if (cb->currentText() != data.toString()) cb->setCurrentText(data.toString());
                        }
                    }
                }
            });

    //  ************ 默认绘制自定义委托，通过paint来绘制非编辑状态下控件样式，使用createEditor绘制编辑控件 ************
    model_paint = new QStandardItemModel(4, 4, this);
    model_paint->setHorizontalHeaderLabels({"姓名", "年龄", "性别", "备注（paint绘制）"});
    model_paint_select = new QItemSelectionModel(model_paint, this);

    ui->paint->setModel(model_paint);
    ui->paint->setSelectionModel(model_paint_select);

    // 设置自定义委托
    CustomTableDelegate *paintDelegate = new CustomTableDelegate(this);
    ui->paint->setItemDelegate(paintDelegate);

    // 填充初始数据
    for (int row = 0; row < 4; ++row) {
        model_paint->setItem(row, 0, new QStandardItem("张三"));
        model_paint->setItem(row, 1, new QStandardItem(QString::number(18 + row)));
        model_paint->setItem(row, 2, new QStandardItem("男"));
        model_paint->setItem(row, 3, new QStandardItem("备注" + QString::number(row)));
    }
    // 全自定义委托，通过paint来绘制非编辑状态下控件样式，使用editorEvent来实现对应的功能
    ui->quanziding->setStyleSheet("background-color:red;");
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
    int rowCount = model_mvc->rowCount();
    int colCount = model_mvc->columnCount();

    QString text;

    // 拼接表头
    QStringList headers;
    for (int c = 0; c < colCount; ++c) {
        headers << model_mvc->horizontalHeaderItem(c)->text();
    }
    text += headers.join("\t") + "\n";
    text += "------------------------------------------------\n";

    // 拼接每一行数据
    for (int r = 0; r < rowCount; ++r) {
        QStringList rowData;
        for (int c = 0; c < colCount; ++c) {
            QModelIndex index = model_mvc->index(r, c);
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
    model_mvc->clear();

    // 2. 重新设置行列数
    model_mvc->setRowCount(4);
    model_mvc->setColumnCount(5);

    // 3. 恢复表头
    model_mvc->setHorizontalHeaderLabels({"列0", "列1", "列2", "列3", "列4"});

    // 4. 恢复初始测试数据
    for(int row = 0; row < 4; row++){
        model_mvc->setItem(row, 0, new QStandardItem("0"));
        model_mvc->setItem(row, 1, new QStandardItem("1"));
        model_mvc->setItem(row, 2, new QStandardItem("2"));
        model_mvc->setItem(row, 3, new QStandardItem("3"));
        model_mvc->setItem(row, 4, new QStandardItem("A"));
    }

}

