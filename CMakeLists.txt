# cmake_minimum_required(VERSION 3.19)
# project(SYS LANGUAGES CXX)

# find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg LinguistTools)

# qt_standard_project_setup()

# qt_add_executable(SYS
#     WIN32 MACOSX_BUNDLE
#     main.cpp
#     mainwindow.cpp
#     mainwindow.h
#     mainwindow.ui
#     userinfo.h userinfo.cpp userinfo.ui
#     res.qrc
# )

# file(GLOB_RECURSE RES_FILES
#     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
#     "${CMAKE_CURRENT_SOURCE_DIR}/res/*"
# )

# qt_add_resources(SYS "resources"
#     PREFIX "/"
#     FILES ${RES_FILES}
# )

# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
#     "${CMAKE_CURRENT_SOURCE_DIR}/res"
# )
# # qt_add_resources(SYS "res"
# #     PREFIX "/"
# #     FILES
# #         res/svg/caiwu.svg
# #         res/svg/class.svg
# #         res/svg/rongyu.svg
# #         res/svg/set1.svg
# #         res/svg/set2.svg
# #         res/svg/user.svg
# # )

# qt_add_translations(
#     TARGETS SYS
#     TS_FILES SYS_zh_CN.ts
# )

# target_link_libraries(SYS
#     PRIVATE
#         Qt::Core
#         Qt::Widgets
#         Qt::Svg
# )

# install(TARGETS SYS
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# qt_generate_deploy_app_script(
#     TARGET SYS
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})

# 1. 指定支持的最小 CMake 版本
cmake_minimum_required(VERSION 3.19)

# 2. 项目名称：SYS，使用 C++ 语言
project(SYS LANGUAGES CXX)

# 3. 找到 Qt6 依赖包，必须包含的模块
# Core：核心功能
# Widgets：界面控件
# Svg：SVG图标支持
# LinguistTools：多语言翻译
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg LinguistTools)

# 4. Qt 标准项目初始化（自动处理 moc/uic/rcc）
qt_standard_project_setup()

# ==============================================
# 🔴 自动搜索所有源码文件（.h .cpp .ui）
# ==============================================
file(GLOB_RECURSE SRC_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

# 5. 排除 build 目录，防止编译报错（解决你刚才的错误）
list(FILTER SRC_FILES EXCLUDE REGEX "(^|/)build/")

# ==============================================
# 🟢 自动搜索所有资源文件
# ==============================================
file(GLOB_RECURSE RES_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/res/*"
)

# ==============================================
# 🟡 自动监视文件夹变化
# 新增文件不用手动运行 CMake，自动识别
# ==============================================
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==============================================
# 🔵 生成可执行程序
# 自动包含所有源码
# ==============================================
qt_add_executable(SYS
    WIN32 MACOSX_BUNDLE  # Windows 不显示黑窗口，Mac 生成 App 包
    ${SRC_FILES}         # 自动引入所有源码
)

# ==============================================
# 🟣 自动打包所有资源文件
# ==============================================
qt_add_resources(SYS "resources"
    PREFIX "/"           # 资源路径前缀：/
    FILES ${RES_FILES}   # 自动引入所有资源
)

# ==============================================
# 🟠 多语言翻译文件配置
# ==============================================
qt_add_translations(
    TARGETS SYS
    TS_FILES SYS_zh_CN.ts
)

# ==============================================
# 🔗 链接 Qt 库（必须和 find_package 对应）
# ==============================================
target_link_libraries(SYS
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
)

# ==============================================
# 🚀 安装配置（发布程序用）
# ==============================================
install(TARGETS SYS
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 自动生成发布脚本（自动拷贝 Qt 依赖库）
qt_generate_deploy_app_script(
    TARGET SYS
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})