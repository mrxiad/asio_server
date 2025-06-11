cmake_minimum_required(VERSION 3.10)
project(YourProjectName)
file(GLOB client_SOURCES "src/*.cpp")

add_executable(client ${client_SOURCES})
target_include_directories(client PUBLIC include)

# 查找 Boost 库
find_package(Boost REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    message(STATUS "找到了 Boost 库 ${Boost_VERSION_STRING} 在 ${Boost_INCLUDE_DIRS}")
    target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(client ${Boost_LIBRARIES})
endif()

# 添加 pthread 库
target_link_libraries(client pthread)

# 查找 jsoncpp 库，优先从 output/lib 中查找
find_library(JSONCPP_LIB jsoncpp
            PATHS "${CMAKE_SOURCE_DIR}/../output/lib" 
            PATH_SUFFIXES "" 
            NO_DEFAULT_PATH)
if(NOT JSONCPP_LIB)
    find_library(JSONCPP_LIB jsoncpp)
endif()
if(JSONCPP_LIB)
    target_link_libraries(client ${JSONCPP_LIB})
else()
    message(FATAL_ERROR "无法找到 jsoncpp 库")
endif()

