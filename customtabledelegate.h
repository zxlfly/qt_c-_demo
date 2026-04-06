#ifndef CUSTOMTABLEDELEGATE_H
#define CUSTOMTABLEDELEGATE_H
#include <QStyledItemDelegate>
class CustomTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CustomTableDelegate(QObject *parent = nullptr);
    // 绘制单元格
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;



    // 创建编辑器
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    // 模型 → 编辑器
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    // 编辑器 → 模型
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
protected:

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;
};

#endif // CUSTOMTABLEDELEGATE_H
