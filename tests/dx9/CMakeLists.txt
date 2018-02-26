project(dx9_tests)

set(SHADERS_RES_FILE "${CMAKE_BINARY_DIR}/resources/shaders.res")
set_source_files_properties(
        "${SHADERS_RES_FILE}"
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)

function(dx9_test name src)
    add_executable(${name} "${src}")
    add_dependencies(${name} static_res)
    add_test(NAME ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/previous
            COMMAND $<TARGET_FILE:${name}> .)
    set_target_properties(
            ${name}
            PROPERTIES
            COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
            LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}"
    )
    target_link_libraries(${name} PUBLIC directgraph_dx9_int directgraph_testlib)
    set_tests_properties(${name} PROPERTIES LABELS "dx9")
endfunction()

set(DX9_BAR_SRC dx9_bar.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar "${DX9_BAR_SRC}")
set(DX9_SINGLE_PIXEL_SRC dx9_single_pixel.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_single_pixel "${DX9_SINGLE_PIXEL_SRC}")
set(DX9_PIXEL_CONTAINER_SRC dx9_pixel_container.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_pixel_container "${DX9_PIXEL_CONTAINER_SRC}")
set(DX9_BAR_FILLSTYLE_SRC dx9_bar_fillstyle.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar_fillstyle "${DX9_BAR_FILLSTYLE_SRC}")
set(DX9_BAR_FILLSTYLE_OLD_SRC dx9_bar_fillstyle_old.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar_fillstyle_old "${DX9_BAR_FILLSTYLE_OLD_SRC}")
set(DX9_FILLELLIPSE_SRC dx9_fillellipse.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse "${DX9_FILLELLIPSE_SRC}")
set(DX9_FILLELLIPSE_OLD_SRC dx9_fillellipse_old.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse_old "${DX9_FILLELLIPSE_OLD_SRC}")