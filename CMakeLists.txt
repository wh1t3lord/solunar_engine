cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(everything)
set (CMAKE_CXX_STANDARD 17)
include(cmake/common.cmake)

# add vendor dir
add_subdirectory(externals)

# add engine dir
add_subdirectory(source)
