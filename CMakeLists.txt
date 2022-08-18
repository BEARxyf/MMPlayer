# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("MMPlayer")
#add_definitions("-DUNICODE" "-D_UNICODE")
#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MMPlayer)
# 包含子项目。
add_subdirectory ("MMPlayer")
add_subdirectory ("MMThread")
add_subdirectory ("MMAV")

