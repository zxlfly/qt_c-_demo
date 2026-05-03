#include "customtabledelegate.h"
#include <QPainter>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QApplication>
#include <QStyleOptionFrame>
#include <QStyleOptionSpinBox>
#include <QStyleOptionComboBox>

CustomTableDelegate::CustomTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void CustomTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    painter->save();
    QStyle *style = QApplication::style();

    // 选中状态背景
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    switch (index.column()) {
    case 0:
    case 3: {
        // 用 QStyle 绘制原生 QLineEdit 外观
        QStyleOptionFrame opt;
        opt.QStyleOption::operator=(option);
        opt.lineWidth = 1;
        opt.midLineWidth = 0;
        opt.features = QStyleOptionFrame::None;
        // Qt6: CE_FrameLineEdit 已移除，改用 PE_FrameLineEdit + PE_PanelLineEdit
        style->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, painter);
        style->drawPrimitive(QStyle::PE_FrameLineEdit, &opt, painter);

        // 在原生边框内绘制文本
        QRect textRect = style->subElementRect(QStyle::SE_LineEditContents, &opt);
        textRect.adjust(2, 0, -2, 0);
        QPalette::ColorRole textRole = (option.state & QStyle::State_Selected)
                                           ? QPalette::HighlightedText : QPalette::Text;
        painter->setPen(option.palette.color(textRole));
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                          index.data(Qt::DisplayRole).toString());
        break;
    }
    case 1: {
        // 用 QStyle 绘制原生 QSpinBox 外观
        QStyleOptionSpinBox opt;
        opt.QStyleOption::operator=(option);
        opt.subControls = QStyle::SC_SpinBoxFrame | QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown;
        opt.activeSubControls = QStyle::SC_None;
        opt.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
        opt.frame = true;
        style->drawComplexControl(QStyle::CC_SpinBox, &opt, painter);

        // 通过 SC_SpinBoxEditField 子控件获取文本区域
        QRect textRect = style->subControlRect(QStyle::CC_SpinBox, &opt,
                                               QStyle::SC_SpinBoxEditField);
        textRect.adjust(2, 0, -2, 0);
        QPalette::ColorRole textRole = (option.state & QStyle::State_Selected)
                                           ? QPalette::HighlightedText : QPalette::Text;
        painter->setPen(option.palette.color(textRole));
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight,
                          index.data(Qt::DisplayRole).toString());
        break;
    }
    case 2: {
        // 用 QStyle 绘制原生 QComboBox 外观
        QStyleOptionComboBox opt;
        opt.QStyleOption::operator=(option);
        opt.subControls = QStyle::SC_ComboBoxFrame | QStyle::SC_ComboBoxArrow;
        opt.activeSubControls = QStyle::SC_None;
        opt.currentText = index.data(Qt::DisplayRole).toString();
        opt.currentIcon = QIcon();
        opt.editable = false;
        opt.frame = true;
        style->drawComplexControl(QStyle::CC_ComboBox, &opt, painter);

        // 选中状态下需要覆盖文本颜色
        if (option.state & QStyle::State_Selected) {
            QRect textRect = style->subControlRect(QStyle::CC_ComboBox, &opt,
                                                   QStyle::SC_ComboBoxEditField);
            textRect.adjust(3, 0, -3, 0);
            painter->setPen(option.palette.color(QPalette::HighlightedText));
            painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, opt.currentText);
        }
        break;
    }
    default:
        QStyledItemDelegate::paint(painter, option, index);
        break;
    }

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
