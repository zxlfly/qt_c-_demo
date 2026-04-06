#include "customlistdelegate.h"
#include <QPainter>
#include <QLineEdit>

CustomListDelegate::CustomListDelegate(QObject *parent): QStyledItemDelegate(parent) {

}
// ====================== 自定义渲染 ======================
void CustomListDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QString text = index.data().toString();

    // 绘制背景（选中时变色）
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(100, 150, 255)); // 蓝色
    } else {
        painter->fillRect(option.rect, QColor(245, 245, 245)); // 灰色
    }

    // 绘制文字（居中）
    painter->drawText(option.rect, Qt::AlignCenter, text);
}

QSize CustomListDelegate::sizeHint(const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    return QSize(option.rect.width(), 40); // 每个item高度固定40
}

// ====================== 自定义编辑 ======================
QWidget *CustomListDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    // 创建编辑器：这里用 QLineEdit，你也可以换成 QComboBox / QSpinBox
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void CustomListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // 把模型数据 → 编辑器
    QString value = index.data().toString();
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(value);
}

void CustomListDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    // 把编辑器数据 → 模型
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    model->setData(index, edit->text());
}