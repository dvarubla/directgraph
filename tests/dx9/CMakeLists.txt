project(dx9_tests)

set(SHADERS_RES_FILE "${OUTPUT_RES}")
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
            LINK_FLAGS "${TESTS_LINK_FLAGS} ${EXTRA_LINK_FLAGS}"
    )
    target_link_libraries(${name} PUBLIC directgraph_dx9_int directgraph_testlib)
    set_tests_properties(${name} PROPERTIES LABELS "dx9")
endfunction()

set(DX9_RECTANGLE_SRC dx9_rectangle.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_rectangle "${DX9_RECTANGLE_SRC}")
set(DX9_RECTANGLE_LINESTYLE_SRC dx9_rectangle_linestyle.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_rectangle_linestyle "${DX9_RECTANGLE_LINESTYLE_SRC}")
set(DX9_BAR_SRC dx9_bar.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar "${DX9_BAR_SRC}")
set(DX9_SINGLE_PIXEL_SRC dx9_single_pixel.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_single_pixel "${DX9_SINGLE_PIXEL_SRC}")
set(DX9_PIXEL_CONTAINER_SRC dx9_pixel_container.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_pixel_container "${DX9_PIXEL_CONTAINER_SRC}")
set(DX9_BAR_FILLSTYLE_SRC dx9_bar_fillstyle.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar_fillstyle "${DX9_BAR_FILLSTYLE_SRC}")
set(DX9_BAR_FILLSTYLE_NO_SHADER dx9_bar_fillstyle_no_shader.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar_fillstyle_no_shader "${DX9_BAR_FILLSTYLE_NO_SHADER}")
set(DX9_BAR_FILLSTYLE_OLD_SRC dx9_bar_fillstyle_old.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_bar_fillstyle_old "${DX9_BAR_FILLSTYLE_OLD_SRC}")
set(DX9_FILLELLIPSE_SRC dx9_fillellipse.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse "${DX9_FILLELLIPSE_SRC}")
set(DX9_FILLELLIPSE_STYLE_SRC dx9_fillellipse_style.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse_style "${DX9_FILLELLIPSE_STYLE_SRC}")
set(DX9_FILLELLIPSE_STYLE_OLD_SRC dx9_fillellipse_style_old.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse_style_old "${DX9_FILLELLIPSE_STYLE_OLD_SRC}")
set(DX9_FILLELLIPSE_OLD_SRC dx9_fillellipse_old.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_fillellipse_old "${DX9_FILLELLIPSE_OLD_SRC}")

set(DX9_TRANSPARENCY_SRC dx9_transparency.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_transparency "${DX9_TRANSPARENCY_SRC}")
set(DX9_TRANSPARENCY_NO_SHADER_SRC dx9_transparency_no_shader.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_transparency_no_shader "${DX9_TRANSPARENCY_NO_SHADER_SRC}")

set(DX9_DIFFERENT_DPI_SRC dx9_different_dpi.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h common.cpp ../stubs/FeaturesStub.h)
dx9_test(dx9_different_dpi "${DX9_DIFFERENT_DPI_SRC}")

set(DX9_INT1_SRC dx9_int1.cpp ${SHADERS_RES_FILE} CommonIntegration.h CommonIntegration.cpp ../stubs/WindowFactoryStub.cpp)
dx9_test(dx9_int1 "${DX9_INT1_SRC}")