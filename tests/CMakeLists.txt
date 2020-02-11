
option(BUILD_BENCHMARK "Build benchmark" ON)

if (BUILD_BENCHMARK)
    if (UNIX)
        set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
        add_subdirectory(vendor/google/benchmark)
        add_executable(histogram_benchmark histogram_benchmark.cpp)
        target_link_libraries(histogram_benchmark tdigest)
        target_link_libraries(histogram_benchmark benchmark)
    else()
        message(WARNING
              "google.benchmark - microbenchmarks disabled on WIN32 platforms")
    endif()
endif()