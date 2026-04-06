#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
class QStringListModel;
class QStandardItemModel;
class QItemSelectionModel;

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

private:
    Ui::MainWindow *ui;
    QStringList m_strList;
    QStringListModel *m_model_list;

    QStandardItemModel *m_model_table;
    QItemSelectionModel *m_model_table_select;
};
#endif // MAINWINDOW_H
