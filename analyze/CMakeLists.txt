add_library(analyzer SHARED WordFrequencyAnalyzer.cpp)
target_include_directories(analyzer PUBLIC ${CMAKE_SOURCE_DIR}/include)