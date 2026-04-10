#include "mvcdetailcomboboxdelegate.h"
#include <QComboBox>
MvcDetailComboBoxDelegate::MvcDetailComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent} {}

void MvcDetailComboBoxDelegate::setMvcDetailComboBoxDelegateList(QStringList items, bool editable)
{
    // 设置拉下选项，以及是否可以使用
    mvcDetailComboBoxDelegateListEditable = editable;
    mvcDetailComboBoxDelegateList = items;
}

QWidget *MvcDetailComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(mvcDetailComboBoxDelegateListEditable);
    for (auto item:mvcDetailComboBoxDelegateList) {
        editor->addItem(item);
    }
    return editor;
}

void MvcDetailComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    QString str = index.model()->data(index,Qt::EditRole).toString();
    comboBox->setCurrentText(str);
}

void MvcDetailComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    QString str = comboBox->currentText();
    model->setData(index,str,Qt::EditRole);
}

void MvcDetailComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}