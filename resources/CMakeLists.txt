project(resources)

if("$ENV{DXSDK_DIR}" STREQUAL "")
    message( FATAL_ERROR "Don't know where DirectXSDK directory is." )
endif()

set(SHADER_SRCS
        shaders/ps_1_4/ellipse.hlsl
        shaders/ps_1_4/textured_bar.hlsl
        shaders/ps_1_4/textured_ellipse.hlsl
        shaders/ps_2_0/textured_rectangle.hlsl
        shaders/ps_1_4/rectangle.hlsl
        shaders/ps_1_4/textured_line.hlsl
        shaders/ps_3_0/ellipse_outline.hlsl
        shaders/ps_3_0/ellipse_with_outline.hlsl
        shaders/ps_3_0/textured_ellipse_with_outline.hlsl

        shaders/vs_1_1/center_bar.hlsl
        shaders/vs_1_1/textured_bar.hlsl
        shaders/vs_1_1/textured_center_bar.hlsl
        shaders/vs_1_1/textured_center_rectangle.hlsl
        shaders/vs_1_1/center_rectangle.hlsl
        shaders/vs_1_1/textured_line.hlsl
        shaders/vs_1_1/ellipse_outline.hlsl
        shaders/vs_1_1/ellipse_with_outline.hlsl
        shaders/vs_1_1/textured_ellipse_with_outline.hlsl
)

set(COMMON_SRCS shaders/common/common_vs.hlsl shaders/common/common_ps.hlsl)

set(INCLUDE_FILE common_res.h)
set(SHADER_RC shaders/shaders.rc)

set(FXC_BIN $ENV{DXSDK_DIR}/Utilities/bin)
if(${bitness} EQUAL 32)
    set(FXC_BIN "${FXC_BIN}/x86")
else()
    set(FXC_BIN "${FXC_BIN}/x64")
endif()
set(FXC_BIN "${FXC_BIN}/fxc.exe")

set(compiled_shaders)
foreach(shader_src ${SHADER_SRCS})
    get_filename_component(shader_path ${shader_src} ABSOLUTE)
    get_filename_component(shader_fname ${shader_src} NAME_WE)
    get_filename_component(shader_dir ${shader_src} DIRECTORY)
    get_filename_component(shader_profile ${shader_dir} NAME)
    set(output_dir "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${shader_dir}")
    set(output "${output_dir}/${shader_fname}.obj")
    file(MAKE_DIRECTORY "${output_dir}")
    add_custom_command(
            OUTPUT "${output}"
            WORKING_DIRECTORY "${output_dir}"
            VERBATIM
            COMMAND
            ${FXC_BIN} /I${CMAKE_CURRENT_SOURCE_DIR}/shaders/common /LD /T${shader_profile} /Fo${output} ${shader_path}
            DEPENDS "${shader_src}" "${COMMON_SRCS}"
    )
    list(APPEND compiled_shaders ${output})
endforeach()

get_filename_component(RC_FILE_PATH ${SHADER_RC} ABSOLUTE)
get_filename_component(RESOURCES_PATH "." ABSOLUTE)
get_filename_component(SHADER_RC_NAME_WE ${SHADER_RC} NAME_WE)

set(OUTPUT_RES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${SHADER_RC_NAME_WE}.o" CACHE INTERNAL "")

rc_compiler_cmd(RC_COMPILER_CMD)

add_custom_command(
        OUTPUT "${OUTPUT_RES}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
        VERBATIM
        COMMAND
        ${RC_COMPILER_CMD}
        DEPENDS
        ${compiled_shaders} ${SHADER_RC} ${INCLUDE_FILE}
)

add_custom_target(static_res DEPENDS "${OUTPUT_RES}")