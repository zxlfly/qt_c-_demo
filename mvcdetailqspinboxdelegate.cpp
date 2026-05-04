#include "mvcdetailqspinboxdelegate.h"
#include <QSpinBox>
#include <QPainter>
#include <QTimer>
MvcDetailQSpinBoxDelegate::MvcDetailQSpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void MvcDetailQSpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    // 编辑期间跳过渲染，避免 Windows 下编辑器和底层 paint 重影
    if (index == m_editingIndex) {
        painter->fillRect(option.rect, option.palette.base());
        return;
    }
    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *MvcDetailQSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    m_editingIndex = index;  // 标记正在编辑的单元格
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
    // 延迟清除编辑标记，确保编辑器完全移除后再恢复 paint() 渲染
    QTimer::singleShot(0, const_cast<MvcDetailQSpinBoxDelegate*>(this), [this]() {
        m_editingIndex = QPersistentModelIndex();
    });
}

void MvcDetailQSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //调整样式
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
