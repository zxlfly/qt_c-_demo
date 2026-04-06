#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QButtonGroup"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "Customlistdelegate.h"

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

    m_model_table =new QStandardItemModel(2,6,this);
    m_model_table_select = new QItemSelectionModel(m_model_table,this);
    ui->tableView->setModel(m_model_table);
    ui->tableView->setSelectionModel(m_model_table_select);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);


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
