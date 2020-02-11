FILE(GLOB c_files "*.c")
FILE(GLOB header_files "*.h")

option(BUILD_SHARED "Build shared library" ON)

if (BUILD_SHARED)
    add_library(mergingdigest SHARED ${c_files} ${header_files})
    target_link_libraries(mergingdigest m)
    target_include_directories(mergingdigest SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    install(TARGETS mergingdigest DESTINATION lib${LIB_SUFFIX})
endif (BUILD_SHARED)
