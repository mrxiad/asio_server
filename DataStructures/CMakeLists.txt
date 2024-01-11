#构建动态库
add_library(container SHARED 
                HashWordContainer.cpp
                ListWordContainer.cpp
                TreeWordContainer.cpp
)
#设置头文件目录
target_include_directories(container PUBLIC ${CMAKE_SOURCE_DIR}/include/DataStructures)