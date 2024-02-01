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

target_link_libraries(client "${CMAKE_SOURCE_DIR}/../3rdParty/json/lib/libjsoncpp.so")

