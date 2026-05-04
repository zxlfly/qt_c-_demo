#include "mvcdetailcomboboxdelegate.h"
#include <QComboBox>
#include <QPainter>
#include <QTimer>
MvcDetailComboBoxDelegate::MvcDetailComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent} {}

void MvcDetailComboBoxDelegate::setMvcDetailComboBoxDelegateList(QStringList items, bool editable)
{
    // 设置拉下选项，以及是否可以使用
    mvcDetailComboBoxDelegateListEditable = editable;
    mvcDetailComboBoxDelegateList = items;
}

void MvcDetailComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    // 编辑期间跳过渲染，避免 Windows 下编辑器和底层 paint 重影
    if (index == m_editingIndex) {
        painter->fillRect(option.rect, option.palette.base());
        return;
    }
    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *MvcDetailComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    m_editingIndex = index;  // 标记正在编辑的单元格
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
    // 延迟清除编辑标记，确保编辑器完全移除后再恢复 paint() 渲染
    QTimer::singleShot(0, const_cast<MvcDetailComboBoxDelegate*>(this), [this]() {
        m_editingIndex = QPersistentModelIndex();
    });
}

void MvcDetailComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
