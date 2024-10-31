cmake_minimum_required (VERSION 3.6)
project (experimental)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_BUILD ON)

# add_subdirectory(polymorphism)
add_subdirectory(UsingVariant)
add_subdirectory(AbstractBasePimpl)
add_subdirectory(PlainAbstractBase)
add_subdirectory(TypeErasure)
add_subdirectory(ParameterPackIndexing)
add_subdirectory(ParameterPackIndexingCheckGenericLambdaFeature)

include_directories ("${PROJECT_SOURCE_DIR}")
