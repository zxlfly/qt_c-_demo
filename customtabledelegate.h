#ifndef CUSTOMTABLEDELEGATE_H
#define CUSTOMTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPersistentModelIndex>

class CustomTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomTableDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    // 跟踪当前正在编辑的单元格，paint() 时跳过该单元格避免重影
    mutable QPersistentModelIndex m_editingIndex;
};

#endif
