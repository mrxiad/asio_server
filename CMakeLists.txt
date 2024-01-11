# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.15)

# 设置项目名称和版本
project(MyComplexProject VERSION 1.0)

# 设置全局 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果没有指定构建类型，则默认设置为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")

# 设置可执行文件和库的输出目录
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "可执行文件输出目录: ${CMAKE_SOURCE_DIR}/bin")
message(STATUS "动态库输出目录: ${CMAKE_SOURCE_DIR}/lib")
message(STATUS "静态库输出目录: ${CMAKE_SOURCE_DIR}/archive")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)  # 可执行文件
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)  # 动态库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/archive)  # 静态库

# 设置 CMake 以查找 Boost 的动态库
set(Boost_USE_STATIC_LIBS OFF)        # 只使用 Boost 的动态库
set(Boost_USE_MULTITHREADED ON)       # 使用多线程版本的 Boost
set(Boost_USE_STATIC_RUNTIME OFF)     # 不使用 Boost 静态运行时库

# 查找 Boost 库
find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)

# 如果找到了 Boost，则包含 Boost 的头文件目录
if(Boost_FOUND)
    message(STATUS "找到了 Boost 库 ${Boost_VERSION_MAJOR}.${Boost_VERSION_MINOR}.${Boost_VERSION_PATCH} 在 ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()


# 添加源代码子目录
add_subdirectory(server)
add_subdirectory(src)

# 根据构建类型添加编译器标志
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra -Werror)  # Debug模式的编译器标志
else()
    add_compile_options(-O2)  # 非Debug模式的优化选项
endif()
