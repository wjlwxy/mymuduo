cmake_minimum_required(VERSION 2.5)

project(mymuduo)

set(CMAKE_CXX_STANDARD 11)

# mymuduo编译为so动态库，设置动态库的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置调试信息
set(CMAKE_BUILD_TYPE Debug)

# 定义参与编译的源代码文件
aux_source_directory(. SRC_LIST)
#编译生成动态库mymuduo
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})