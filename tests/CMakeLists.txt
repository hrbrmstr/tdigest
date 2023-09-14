if (BUILD_TESTS)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c99")
    add_executable(td_test unit/td_test.c unit/minunit.h)
    target_link_libraries(td_test tdigest m)
    enable_testing()
    add_test(td_test td_test)
endif()



