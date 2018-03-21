project(main_tests)

function(main_test name src)
    add_executable(${name} "${src}")
    set_target_properties(
            ${name}
            PROPERTIES
            COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
            LINK_FLAGS "${TESTS_LINK_FLAGS} ${EXTRA_LINK_FLAGS}"
    )
    target_link_libraries(${name} PUBLIC directgraph_common directgraph_testlib)
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}> .)
endfunction()

set(THREADS_SIMPLE_SRC threads_simple.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)
main_test(threads_simple "${THREADS_SIMPLE_SRC}")
set(THREADS_LAST_SRC threads_last.cpp ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/QueueItemUtils.cpp ../stubs/QueueItemUtils.h)
main_test(threads_last "${THREADS_LAST_SRC}")
set(WINDOW_MANAGER_SRC window_manager.cpp window_manager.cpp ../stubs/WindowFactoryStub.cpp ../stubs/WindowFactoryStub.h ../stubs/MyWindowStub.cpp ../stubs/MyWindowStub.h ../stubs/RendererStub.cpp ../stubs/RendererStub.h ../stubs/ControllerFactoryStub.cpp ../stubs/ControllerFactoryStub.h ../stubs/ControllerStub.cpp ../stubs/ControllerStub.h)
main_test(window_manager "${WINDOW_MANAGER_SRC}")