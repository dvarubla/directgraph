project(main_tests)

add_executable(pixel_container pixel_container.cpp)
add_executable(threads_simple threads_simple.cpp threads_simple.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h)

set_target_properties(
    pixel_container threads_simple
    PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
    LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}"
)

target_link_libraries(pixel_container PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(threads_simple PUBLIC directgraph_common directgraph_testlib)