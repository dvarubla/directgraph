project(main_tests)

add_executable(pixel_container pixel_container.cpp)
add_executable(threads_simple threads_simple.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)
add_executable(threads_last threads_last.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)

set_target_properties(
    pixel_container threads_simple threads_last
    PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
    LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}"
)

target_link_libraries(pixel_container PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(threads_simple PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(threads_last PUBLIC directgraph_common directgraph_testlib)