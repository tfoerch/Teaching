cmake_minimum_required (VERSION 3.6)
project (experimental)
option(USE_CLANG "build application with clang" OFF) # OFF is the default
if(USE_CLANG)
SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++-17")
SET (CMAKE_C_COMPILER               "/usr/bin/clang-17")
else(USE_CLANG)
SET (CMAKE_CXX_COMPILER             "/usr/bin/g++-13")
SET (CMAKE_C_COMPILER               "/usr/bin/gcc-13")
endif(USE_CLANG)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy-17")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_BUILD ON)

# add_subdirectory(polymorphism)
# add_subdirectory(UsingVariant)
# add_subdirectory(AbstractBasePimpl)
# add_subdirectory(PlainAbstractBase)
# add_subdirectory(TypeErasure)
add_subdirectory(ParameterPackIndexing)
add_subdirectory(ParameterPackIndexingCheckGenericLambdaFeature)

include_directories ("${PROJECT_SOURCE_DIR}")
