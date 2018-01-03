project(main_tests)

add_executable(pixel_container pixel_container.cpp)
add_executable(threads_simple threads_simple.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)
add_executable(threads_last threads_last.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)
add_executable(window_manager window_manager.cpp window_manager.cpp ../stubs/WindowFactoryStub.cpp ../stubs/WindowFactoryStub.h ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h)

set_target_properties(
    pixel_container threads_simple threads_last window_manager
    PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
    LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}"
)

target_link_libraries(pixel_container PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(threads_simple PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(threads_last PUBLIC directgraph_common directgraph_testlib)
target_link_libraries(window_manager PUBLIC directgraph_common directgraph_testlib)

add_test(NAME pixel_container COMMAND $<TARGET_FILE:pixel_container> .)
add_test(NAME threads_simple COMMAND $<TARGET_FILE:threads_simple> .)
add_test(NAME threads_last COMMAND $<TARGET_FILE:threads_last> .)
add_test(NAME window_manager COMMAND $<TARGET_FILE:threads_last> .)