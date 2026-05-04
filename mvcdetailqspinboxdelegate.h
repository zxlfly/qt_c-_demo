#ifndef MVCDETAILQSPINBOXDELEGATE_H

#define MVCDETAILQSPINBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPersistentModelIndex>

class MvcDetailQSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MvcDetailQSpinBoxDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    mutable QPersistentModelIndex m_editingIndex;
};


#endif // MVCDETAILQSPINBOXDELEGATE_H
