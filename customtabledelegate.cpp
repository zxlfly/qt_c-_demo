#include "customtabledelegate.h"
#include <QPainter>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

CustomTableDelegate::CustomTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void CustomTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
    case 3:
        paintLineEditStyle(painter, option, index);
        break;
    case 1:
        paintSpinBoxStyle(painter, option, index);
        break;
    case 2:
        paintComboBoxStyle(painter, option, index);
        break;
    default:
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void CustomTableDelegate::paintLineEditStyle(QPainter *painter,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    } else {
        painter->fillRect(option.rect, Qt::white);
    }

    painter->setPen(QPen(Qt::gray, 1));
    painter->drawRect(option.rect.adjusted(1, 1, -1, -1));

    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(option.state & QStyle::State_Selected
                        ? option.palette.highlightedText().color()
                        : Qt::black);
    painter->drawText(option.rect.adjusted(5, 0, -5, 0),
                      Qt::AlignVCenter | Qt::AlignLeft,
                      text);

    painter->restore();
}

void CustomTableDelegate::paintSpinBoxStyle(QPainter *painter,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    painter->save();

    QRect rect = option.rect;

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, option.palette.highlight());
    } else {
        painter->fillRect(rect, Qt::white);
    }

    painter->setPen(QPen(Qt::gray, 1));
    painter->drawRect(rect.adjusted(1, 1, -1, -1));

    int btnWidth = 20;
    QRect btnRect(rect.right() - btnWidth, rect.top(), btnWidth, rect.height());
    painter->fillRect(btnRect, QColor(240, 240, 240));
    painter->drawLine(btnRect.left(), btnRect.top(), btnRect.left(), btnRect.bottom());

    QString text = index.data(Qt::DisplayRole).toString();
    QRect textRect = rect.adjusted(5, 0, -btnWidth - 5, 0);
    painter->setPen(option.state & QStyle::State_Selected
                        ? option.palette.highlightedText().color()
                        : Qt::black);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, text);

    painter->restore();
}

void CustomTableDelegate::paintComboBoxStyle(QPainter *painter,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    painter->save();

    QRect rect = option.rect;

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, option.palette.highlight());
    } else {
        painter->fillRect(rect, Qt::white);
    }

    painter->setPen(QPen(Qt::gray, 1));
    painter->drawRect(rect.adjusted(1, 1, -1, -1));

    int arrowWidth = 20;
    QRect arrowRect(rect.right() - arrowWidth, rect.top(), arrowWidth, rect.height());
    painter->fillRect(arrowRect, QColor(240, 240, 240));
    painter->drawLine(arrowRect.left(), arrowRect.top(), arrowRect.left(), arrowRect.bottom());

    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    QPoint points[3] = {
        QPoint(arrowRect.center().x() - 3, arrowRect.center().y() - 2),
        QPoint(arrowRect.center().x() + 3, arrowRect.center().y() - 2),
        QPoint(arrowRect.center().x(), arrowRect.center().y() + 2)
    };
    painter->drawPolygon(points, 3);

    QString text = index.data(Qt::DisplayRole).toString();
    QRect textRect = rect.adjusted(5, 0, -arrowWidth - 5, 0);
    painter->setPen(option.state & QStyle::State_Selected
                        ? option.palette.highlightedText().color()
                        : Qt::black);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    painter->restore();
}

QWidget *CustomTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
    case 3: {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    case 1: {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(0, 150);
        return editor;
    }
    case 2: {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"男", "女"});
        return editor;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void CustomTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.data(Qt::EditRole);

    if (QLineEdit *le = qobject_cast<QLineEdit*>(editor)) {
        le->setText(value.toString());
    } else if (QSpinBox *sb = qobject_cast<QSpinBox*>(editor)) {
        sb->setValue(value.toInt());
    } else if (QComboBox *cb = qobject_cast<QComboBox*>(editor)) {
        cb->setCurrentText(value.toString());
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void CustomTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const
{
    QVariant value;

    if (QLineEdit *le = qobject_cast<QLineEdit*>(editor)) {
        value = le->text();
    } else if (QSpinBox *sb = qobject_cast<QSpinBox*>(editor)) {
        value = sb->value();
    } else if (QComboBox *cb = qobject_cast<QComboBox*>(editor)) {
        value = cb->currentText();
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }

    model->setData(index, value, Qt::EditRole);
}

void CustomTableDelegate::updateEditorGeometry(QWidget *editor,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
