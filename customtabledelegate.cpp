#include "customtabledelegate.h"
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QApplication>
#include <QListView>
CustomTableDelegate::CustomTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}
// ==========================
// 绘制：复选框 + 图片文字
// ==========================
void CustomTableDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    int col = index.column();

    // ---------- 第3列：复选框显示 ----------
    if (col == 3) {
        bool checked = index.data().toBool();
        QStyleOptionButton opt;
        opt.rect = option.rect.adjusted(15, 8, -15, -8);
        opt.state = checked ? QStyle::State_On : QStyle::State_Off;
        opt.state |= QStyle::State_Enabled;
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
        return;
    }

    // ---------- 第5列：图片 + 文字 ----------
    if (col == 5) {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // 画一个小图标（你可以换成自己的图片）
        QRect iconR = option.rect.adjusted(4, 4, -60, -4);
        painter->drawPixmap(iconR, QPixmap(":/img.png"));

        // 画文字
        QRect textR = option.rect.adjusted(35, 0, 0, 0);
        painter->drawText(textR, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
        return;
    }

    // 其他列默认绘制
    QStyledItemDelegate::paint(painter, option, index);
}

// ==========================
// 创建编辑器：按列分配
// ==========================
QWidget *CustomTableDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    int col = index.column();

    // 0列：文本
    if (col == 0) return new QLineEdit(parent);

    // 1列：下拉框
    if (col == 1) {
        // auto *cb = new QComboBox(parent);
        // cb->addItems({"北京","上海","广州","深圳","成都"});
        // cb->setView(new QListView()); // 解决显示重叠问题
        // return cb;
        auto *cb = new QComboBox(parent);
        // 1. 填充下拉选项
        cb->addItems({"北京","上海","广州","深圳","成都"});

        // 2. 解决重叠的核心配置（Qt 6.1.0 兼容）
        QListView *listView = new QListView(cb);
        listView->setUniformItemSizes(true); // 统一选项大小，避免布局错乱
        listView->setResizeMode(QListView::Adjust); // 自适应内容宽度
        cb->setView(listView);

        // 3. 优化样式和大小，避免重叠
        // cb->setSizeAdjustPolicy(QComboBox::AdjustToContents); // 宽度适配内容
        // cb->setMinimumWidth(option.rect.width()); // 强制匹配单元格宽度
        // cb->setMinimumHeight(option.rect.height()); // 强制匹配单元格高度
        // 去除默认边框和内边距，避免挤压内容
        // cb->setStyleSheet(R"(
        //     QComboBox {
        //         padding: 1px;
        //         border: none;
        //         background: white;
        //     }
        //     QComboBox QAbstractItemView {
        //         border: 1px solid #ccc;
        //         padding: 0px;
        //         margin-top: 2px; /* 下拉列表与编辑器拉开一点距离，彻底避免重叠 */
        //     }
        // )");

        return cb;
    }

    // 2列：数字框
    if (col == 2) {
        auto *sb = new QSpinBox(parent);
        sb->setRange(0, 100);
        return sb;
    }

    // 4列：输入框
    if (col == 4) return new QLineEdit(parent);

    // 3列复选框、5列图片：不需要编辑器
    return nullptr;
}

// ==========================
// 模型 → 编辑器
// ==========================
void CustomTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int col = index.column();
    auto d = index.data();

    if (col == 0) qobject_cast<QLineEdit*>(editor)->setText(d.toString());
    else if (col == 1) qobject_cast<QComboBox*>(editor)->setCurrentText(d.toString());
    else if (col == 2) qobject_cast<QSpinBox*>(editor)->setValue(d.toInt());
    else if (col == 4) qobject_cast<QLineEdit*>(editor)->setText(d.toString());
}

// ==========================
// 编辑器 → 模型
// ==========================
void CustomTableDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    int col = index.column();

    if (col == 0) model->setData(index, qobject_cast<QLineEdit*>(editor)->text());
    else if (col == 1) model->setData(index, qobject_cast<QComboBox*>(editor)->currentText());
    else if (col == 2) model->setData(index, qobject_cast<QSpinBox*>(editor)->value());
    else if (col == 4) model->setData(index, qobject_cast<QLineEdit*>(editor)->text());
}

// ==========================
// 编辑器位置
// ==========================
void CustomTableDelegate::updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    if (QComboBox *cb = qobject_cast<QComboBox*>(editor)) {
        cb->adjustSize();
    }
}
// ==========================
// 修复：复选框点击切换（补全const，匹配头文件签名）
// ==========================
bool CustomTableDelegate::editorEvent(QEvent *event,
                                      QAbstractItemModel *model,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index)
{
    if (index.column() == 3) {
        if (event->type() == QEvent::MouseButtonPress) {
            bool isChecked = index.data(Qt::EditRole).toBool();
            model->setData(index, !isChecked, Qt::EditRole);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}