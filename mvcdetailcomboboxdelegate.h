#ifndef MVCDETAILCOMBOBOXDELEGATE_H
#define MVCDETAILCOMBOBOXDELEGATE_H
#include <QStyledItemDelegate>
#include <QObject>
#include <QPersistentModelIndex>

class MvcDetailComboBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MvcDetailComboBoxDelegate(QObject *parent = nullptr);
private:
    QStringList mvcDetailComboBoxDelegateList;
    bool mvcDetailComboBoxDelegateListEditable;
    mutable QPersistentModelIndex m_editingIndex;
    // QAbstractItemDelegate interface
public:
    void setMvcDetailComboBoxDelegateList(QStringList items,bool editable);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // MVCDETAILCOMBOBOXDELEGATE_H
