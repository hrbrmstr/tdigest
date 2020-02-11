FILE(GLOB c_files "*.c")
FILE(GLOB header_files "*.h")

option(BUILD_SHARED "Build shared library" ON)

if (BUILD_SHARED)
    add_library(tdigest SHARED ${c_files} ${header_files})
    target_link_libraries(tdigest m)
    target_include_directories(tdigest SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    install(TARGETS tdigest DESTINATION lib${LIB_SUFFIX})
endif (BUILD_SHARED)
