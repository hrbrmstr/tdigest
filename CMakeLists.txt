if (MSVC)
    cmake_minimum_required (VERSION 3.7.0)
else ()
    cmake_minimum_required (VERSION 2.8.11)
endif ()

# http://www.cmake.org/cmake/help/v3.0/policy/CMP0048.html
if (POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
endif()

macro (add_compiler_flags)
    string (REPLACE ";" " " _FLAGS "${ARGV}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}")
endmacro ()

project(tdigest VERSION 0.4.0 LANGUAGES C)

ENABLE_TESTING()

set(CMAKE_BUILD_TYPE Release)

option(BUILD_SHARED "Build shared library" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_BENCHMARK "Build benchmark" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

add_compiler_flags ("-fPIC")

option (ENABLE_FRAME_POINTER "Disable frame pointer omission" ON)

if (ENABLE_FRAME_POINTER)
    # disable frame pointer omission
    if (MSVC)
        add_compiler_flags (/Oy-)
    else ()
        add_compiler_flags (-fno-omit-frame-pointer)
    endif ()
endif ()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")



if(BUILD_TESTS)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif(BUILD_TESTS)

if(BUILD_EXAMPLES)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif(BUILD_EXAMPLES)

