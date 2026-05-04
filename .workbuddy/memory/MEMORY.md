# QT/SYS 项目记忆

## 项目概述
Qt Model/View/Delegate 学习项目，包含 5 种实现方式对比：
1. ListView + 自定义委托
2. 基础表格（不带 Delegate）
3. 基础表格（带 Delegate：SpinBox + ComboBox）
4. 自定义 paint 绘制委托（CustomTableDelegate）
5. 不用委托（setIndexWidget）

## Qt 版本与构建
- Qt 6.10.2 + MinGW 13.1.0 (64-bit)
- 构建目录：`build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/`
- make 路径：`/c/Qt/Tools/mingw1310_64/bin/mingw32-make.exe`

## 关键经验（踩坑记录）

### paint() 和编辑器的关系
- **核心**：编辑器是叠加层(overlay)，paint() 仍在执行，Windows 下会导致重影
- macOS 不重影是因为 Cocoa 控件默认不透明；Windows 控件可能半透明
- 解决：用 QPersistentModelIndex 跟踪编辑单元格，paint() 中跳过渲染
- `QStyle::State_Editing` 在 QTableView 调用 paint() 时不会被设置，不能用

### QStyle 绘制控件
- CC_ComboBox 只画框架和箭头，文本必须额外调用 CE_ComboBoxLabel
- 不要用 initFrom() 初始化 QStyleOption（焦点变化导致 palette 突变），手动设置 state 和 palette
- 强制 State_Enabled | State_Active + Active 色组，确保渲染稳定

### setIndexWidget 的局限
- Widget 是叠加层，不会禁用底层 paint()
- 本质是"Widget 为主 + Model 为副本"的手动双向同步模式
- 需要 NoPaintDelegate 压制底层 paint 的文字绘制

## 修改的文件清单
- customtabledelegate.h/cpp — 完整重写，paint 用 QStyle 绘制 + editingIndex 防重影
- mvcdetailcomboboxdelegate.h/cpp — 加 paint() + editingIndex
- mvcdetailqspinboxdelegate.h/cpp — 加 paint() + editingIndex
- mainwindow.cpp — NoPaintDelegate + setEditTriggers
