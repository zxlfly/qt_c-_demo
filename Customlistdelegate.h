#ifndef CUSTOMLISTDELEGATE_H
#define CUSTOMLISTDELEGATE_H
#include <QStyledItemDelegate>
#include <QObject>

class CustomListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomListDelegate(QObject *parent = nullptr);
    // ====================== 渲染（显示） ======================
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    // ====================== 编辑 ======================
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;
};

#endif // CUSTOMLISTDELEGATE_H
