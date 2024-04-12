cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(everything)

option(USE_SHARED_LIBS "Use engine as shared library." ON)

include(cmake/common.cmake)

# add vendor dir
add_subdirectory(externals)

# add engine dir
add_subdirectory(source)
