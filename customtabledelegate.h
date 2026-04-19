#ifndef CUSTOMTABLEDELEGATE_H
#define CUSTOMTABLEDELEGATE_H

#include <QStyledItemDelegate>

class CustomTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomTableDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    // 辅助绘制方法
    void paintLineEditStyle(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    void paintSpinBoxStyle(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
    void paintComboBoxStyle(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

#endif
