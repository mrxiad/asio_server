cmake_minimum_required(VERSION 3.10)
project(YourProjectName)
file(GLOB SERVER_SOURCES "src/*.cpp")
# 设置全局 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(server ${SERVER_SOURCES})
target_include_directories(server PUBLIC include)

# 查找 Boost 库
find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    message(STATUS "找到了 Boost 库 ${Boost_VERSION_STRING} 在 ${Boost_INCLUDE_DIRS}")
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
endif()

# 添加 pthread 库
target_link_libraries(server pthread)

#json库
target_link_libraries(server "${CMAKE_SOURCE_DIR}/../3rdParty/json/lib/libjsoncpp.so")

