#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
class QStringListModel;
class QStandardItemModel;
class QItemSelectionModel;
class MvcDetailComboBoxDelegate;
class MvcDetailQSpinBoxDelegate;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    MvcDetailComboBoxDelegate* mvcDetailComboBoxDelegate;
    MvcDetailQSpinBoxDelegate* mvcDetailQSpinBoxDelegate;
private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_checkBox_clicked(bool checked);

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();


    void on_pushButton_10_clicked(bool checked);

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void do_currentChange_table(const QModelIndex &current,const QModelIndex &pre);

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QStringList m_strList;
    QStringListModel *m_model_list;

    // 基础
    QStandardItemModel *model_jichu;
    QItemSelectionModel *model_jichu_select;
    // mvc完整基础示例
    QStandardItemModel *model_mvc;
    QItemSelectionModel *model_mvc_select;
};
#endif // MAINWINDOW_H
