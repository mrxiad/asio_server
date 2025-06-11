cmake_minimum_required(VERSION 3.10)
project(YourProjectName)
file(GLOB SERVER_SOURCES "src/*.cpp")
# 设置全局 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(server ${SERVER_SOURCES})


# 添加头文件搜索路径
target_include_directories(server PUBLIC 
        include
        ../public/db
        ../public
        ../model
        /usr/include/mysql)  # 添加 MySQL 开发库的头文件路径

# 查找 Boost 库
find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    message(STATUS "找到了 Boost 库 ${Boost_VERSION_STRING} 在 ${Boost_INCLUDE_DIRS}")
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
endif()

# 添加 pthread 库
target_link_libraries(server pthread)


message(STATUS "当前 CMakeLists.txt 的目录: ${CMAKE_CURRENT_SOURCE_DIR}")

set(LIB_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../output/lib")

# 查找 JSONCPP 库
find_library(JSONCPP_LIB jsoncpp PATHS ${LIB_OUTPUT_DIR} NO_DEFAULT_PATH)
if(NOT JSONCPP_LIB)
    find_library(JSONCPP_LIB jsoncpp)
endif()
if(JSONCPP_LIB)
    target_link_libraries(server ${JSONCPP_LIB})
else()
    message(FATAL_ERROR "无法找到 jsoncpp 库")
endif()

# 查找自定义公共库
find_library(PUBLIC_LIB NAMES lib_public PATHS ${LIB_OUTPUT_DIR} REQUIRED NO_DEFAULT_PATH)
target_link_libraries(server ${PUBLIC_LIB})

# 查找自定义 MySQL 库
find_library(MYSQL_HELPER_LIB NAMES lib_mysql PATHS ${LIB_OUTPUT_DIR} REQUIRED NO_DEFAULT_PATH)
target_link_libraries(server ${MYSQL_HELPER_LIB})


# 直接链接系统的 MySQL 客户端库
find_library(MYSQL_CLIENT mysqlclient)
if(MYSQL_CLIENT)
    target_link_libraries(server ${MYSQL_CLIENT})
else()
    message(FATAL_ERROR "无法找到 MySQL 客户端库")
endif()
