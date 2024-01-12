file(GLOB SERVER_SOURCES "src/*.cpp")

add_executable(server ${SERVER_SOURCES})

target_include_directories(server PUBLIC include)

# 添加boost库
if(Boost_FOUND)
    message(STATUS "找到了Boost库 ${Boost_VERSION_MAJOR}.${Boost_VERSION_MINOR}.${Boost_VERSION_PATCH} in ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost库的库目录是 ${Boost_LIBRARY_DIRS}")
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
endif()

# 添加pthread库
target_link_libraries(server pthread)