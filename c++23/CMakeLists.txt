cmake_minimum_required (VERSION 3.3)
project (experimental)
option(USE_CLANG "build application with clang" OFF) # OFF is the default
if(USE_CLANG)
SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++-16")
SET (CMAKE_C_COMPILER               "/usr/bin/clang-16")
else(USE_CLANG)
SET (CMAKE_CXX_COMPILER             "/usr/bin/g++-13")
SET (CMAKE_C_COMPILER               "/usr/bin/gcc-13")
endif(USE_CLANG)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_BUILD ON)

add_subdirectory(linear-algebra)

include_directories ("${PROJECT_SOURCE_DIR}")
