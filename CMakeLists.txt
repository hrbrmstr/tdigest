cmake_minimum_required(VERSION 3.0)

project(mergingdigest VERSION 0.0.1 DESCRIPTION "Ted Dunning's T-Digest merging-digest version")
set(CMAKE_BUILD_TYPE Release)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")


option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif(BUILD_TESTS)

option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif(BUILD_EXAMPLES)
