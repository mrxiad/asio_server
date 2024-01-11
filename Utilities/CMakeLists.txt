add_library(utils SHARED StringUtils.cpp)
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include/Utilities)