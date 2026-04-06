#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QButtonGroup"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "customlistdelegate.h"
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
        m_model_table->setItem(row, 3, new QStandardItem(Qt::Checked)); // 勾选状态
        m_model_table->setItem(row, 4, new QStandardItem("测试备注"));
        m_model_table->setItem(row, 5, new QStandardItem("图片"));
    }

    m_model_table_select = new QItemSelectionModel(m_model_table,this);
    ui->tableView->setModel(m_model_table);
    ui->tableView->setSelectionModel(m_model_table_select);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setItemDelegate(new CustomTableDelegate(this));
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

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

