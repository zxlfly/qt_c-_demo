#include "mvcdetailqspinboxdelegate.h"
#include <QSpinBox>
MvcDetailQSpinBoxDelegate::MvcDetailQSpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *MvcDetailQSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMaximum(0);
    editor->setMaximum(9999);
    return editor;
}

void MvcDetailQSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // 设置编辑控件显示的值，数据从model中获取，会在源文件中设置
    QSpinBox *spinBox = dynamic_cast<QSpinBox*>(editor);
    int value = index.model()->data(index,Qt::EditRole).toInt();
    spinBox->setValue(value);
}

void MvcDetailQSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // 编辑控件编辑反向修改model
    QSpinBox *spinbox = dynamic_cast<QSpinBox*>(editor);
    int value = spinbox->value();
    model->setData(index,value,Qt::EditRole);
}

void MvcDetailQSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //调整样式
    editor->setGeometry(option.rect);
}