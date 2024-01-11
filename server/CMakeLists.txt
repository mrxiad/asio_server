add_library(server SHARED server.cpp)

message(STATUS "json头文件目录：${CMAKE_SOURCE_DIR}/3rdParty/json/include")
target_include_directories(server PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdParty/json/include)

# 添加json库
target_link_libraries(server "${CMAKE_SOURCE_DIR}/3rdParty/json/lib/libjsoncpp.so")

# 添加boost库
if(Boost_FOUND)
    message(STATUS "找到了Boost库 ${Boost_VERSION_MAJOR}.${Boost_VERSION_MINOR}.${Boost_VERSION_PATCH} in ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost库的库目录是 ${Boost_LIBRARY_DIRS}")
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
endif()

# 添加pthread库
target_link_libraries(server pthread)