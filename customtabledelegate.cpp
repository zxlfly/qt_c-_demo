#include "customtabledelegate.h"
#include <QPainter>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QApplication>
#include <QTimer>
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
    // 编辑器打开期间，跳过自定义渲染，只画背景色，避免编辑器和 paint() 重影
    if (index == m_editingIndex) {
        painter->fillRect(option.rect, option.palette.base());
        return;
    }

    painter->save();

    QStyle *style = option.widget ? option.widget->style() : QApplication::style();
    QString text = index.data(Qt::DisplayRole).toString();

    // 用控件背景色覆盖选中高亮（和 setIndexWidget 效果一致：控件遮住选中色）
    painter->fillRect(option.rect, option.palette.base());

    // 所有控件统一的状态和调色板：
    // - 始终 Enabled | Active，避免编辑后焦点变化导致样式突变（如箭头消失）
    // - 使用 option.palette 并强制 Active 色组，确保颜色一致
    const QStyle::State stableState = QStyle::State_Enabled | QStyle::State_Active;
    QPalette stablePalette = option.palette;
    stablePalette.setCurrentColorGroup(QPalette::Active);

    switch (index.column()) {
    case 0:
    case 3: { // QLineEdit 外观
        QStyleOptionFrame opt;
        opt.rect = option.rect;
        opt.state = stableState;
        opt.palette = stablePalette;
        opt.fontMetrics = option.fontMetrics;
        opt.direction = option.direction;
        opt.lineWidth = 1;
        opt.midLineWidth = 0;
        opt.features = QStyleOptionFrame::None;

        style->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, painter, option.widget);
        style->drawPrimitive(QStyle::PE_FrameLineEdit, &opt, painter, option.widget);

        // 在边框内绘制文本
        QRect textRect = style->subElementRect(QStyle::SE_LineEditContents, &opt, option.widget);
        textRect.adjust(2, 0, -2, 0);
        painter->setPen(stablePalette.color(QPalette::Text));
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
        break;
    }
    case 1: { // QSpinBox 外观
        QStyleOptionSpinBox opt;
        opt.rect = option.rect;
        opt.state = stableState;
        opt.palette = stablePalette;
        opt.fontMetrics = option.fontMetrics;
        opt.direction = option.direction;
        opt.subControls = QStyle::SC_SpinBoxFrame | QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown;
        opt.activeSubControls = QStyle::SC_None;
        opt.stepEnabled = QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
        opt.frame = true;
        opt.buttonSymbols = QAbstractSpinBox::UpDownArrows;

        style->drawComplexControl(QStyle::CC_SpinBox, &opt, painter, option.widget);

        // 在编辑区域绘制数字文本
        QRect textRect = style->subControlRect(QStyle::CC_SpinBox, &opt,
                                                QStyle::SC_SpinBoxEditField, option.widget);
        textRect.adjust(2, 0, -2, 0);
        painter->setPen(stablePalette.color(QPalette::Text));
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, text);
        break;
    }
    case 2: { // QComboBox 外观
        QStyleOptionComboBox opt;
        opt.rect = option.rect;
        opt.state = stableState;
        opt.palette = stablePalette;
        opt.fontMetrics = option.fontMetrics;
        opt.direction = option.direction;
        opt.subControls = QStyle::SC_ComboBoxFrame | QStyle::SC_ComboBoxArrow;
        opt.activeSubControls = QStyle::SC_None;
        opt.currentText = text;
        opt.currentIcon = QIcon();
        opt.editable = false;
        opt.frame = true;
        opt.iconSize = QSize(16, 16);

        // CC_ComboBox 画框架和箭头，CE_ComboBoxLabel 画文本
        style->drawComplexControl(QStyle::CC_ComboBox, &opt, painter, option.widget);
        style->drawControl(QStyle::CE_ComboBoxLabel, &opt, painter, option.widget);
        break;
    }
    default:
        QStyledItemDelegate::paint(painter, option, index);
        break;
    }

    painter->restore();
}

QSize CustomTableDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(qMax(size.height(), 24));
    return size;
}

QWidget *CustomTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    Q_UNUSED(option);
    m_editingIndex = index;  // 标记正在编辑的单元格
    switch (index.column()) {
    case 0:
    case 3:
        return new QLineEdit(parent);
    case 1: {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(0, 150);
        editor->setFrame(true);
        editor->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        return editor;
    }
    case 2: {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"男", "女"});
        editor->setFrame(true);
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

    // 延迟清除编辑标记：确保编辑器完全移除后再恢复 paint() 渲染
    // 避免编辑器关闭和 paint() 同时出现的重影
    QTimer::singleShot(0, const_cast<CustomTableDelegate*>(this), [this]() {
        m_editingIndex = QPersistentModelIndex();
    });
}

void CustomTableDelegate::updateEditorGeometry(QWidget *editor,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
