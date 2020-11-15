## cmake flags
cmake_minimum_required (VERSION 3.11)
project(tdigest)

# CMake modules should be included in ${CMAKE_SOURCE_DIR}/cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# --- Build options ---
option(BUILD_SHARED "Build shared library" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_BENCHMARK "Build benchmark" ON)
option(BUILD_TESTS "Build tests" ON)
OPTION(ENABLE_CODECOVERAGE "Enable code coverage testing support" OFF)
OPTION(ENABLE_PROFILE "Enable code profiling support" OFF)
option(BUILD_EXAMPLES "Build examples" ON)

# --- Build properties ---

# Set a default build type if none was specified
set(default_build_type "Release")

IF(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF()

if(ENABLE_CODECOVERAGE)
  message(STATUS "Forcing build type to Debug to run coverage.")
      set(CMAKE_BUILD_TYPE "Debug" CACHE
              STRING "Choose the type of build." FORCE)
ENDIF()

# Generate position-independent code (-fPIC on UNIX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(ENABLE_PROFILE)
  message(STATUS "Enabling profile flags.")
  string (REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  string (REPLACE "-O3" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  string (REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  string (REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -g -ggdb -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2  -g -ggdb -fno-omit-frame-pointer")
endif(ENABLE_PROFILE)

# --- System Libraries ---
include(GNUInstallDirs)
include(UseCodeCoverage)

include(GNUInstallDirs)

# --- Build directories ---
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

# --- Documentation ---
# TODO 

# --- Unit Tests ---
ENABLE_TESTING()

if(BUILD_TESTS)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif(BUILD_TESTS)

# --- Examples ---
if(BUILD_EXAMPLES)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif(BUILD_EXAMPLES)
