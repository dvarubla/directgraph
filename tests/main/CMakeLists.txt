project(main_tests)

add_executable(pixel_container pixel_container.cpp)

set_target_properties(
    pixel_container
    PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
    LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}"
)

target_link_libraries(pixel_container PUBLIC directgraph_common directgraph_testlib)