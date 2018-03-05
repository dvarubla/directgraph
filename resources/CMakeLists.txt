project(resources)

if("$ENV{DXSDK_DIR}" STREQUAL "")
    message( FATAL_ERROR "Don't know where DirectXSDK directory is." )
endif()

set(SHADER_SRCS
        shaders/ps_1_4/ellipse.hlsl
        shaders/ps_1_4/textured_bar.hlsl
        shaders/ps_1_4/textured_ellipse.hlsl

        shaders/vs_1_1/center_bar.hlsl
        shaders/vs_1_1/textured_bar.hlsl
        shaders/vs_1_1/textured_center_bar.hlsl
)
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
            ${FXC_BIN} /LD /T${shader_profile} /Fo${output} ${shader_path}
            DEPENDS "${shader_src}"
    )
    list(APPEND compiled_shaders ${output})
endforeach()

get_filename_component(RC_FILE_PATH ${SHADER_RC} ABSOLUTE)
get_filename_component(RESOURCES_PATH "." ABSOLUTE)
get_filename_component(SHADER_RC_NAME_WE ${SHADER_RC} NAME_WE)

set(OUTPUT_RES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${SHADER_RC_NAME_WE}.res" CACHE INTERNAL "")

add_custom_command(
        OUTPUT "${OUTPUT_RES}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
        VERBATIM
        COMMAND
            ${CMAKE_RC_COMPILER}
            /i${RESOURCES_PATH} /i${CMAKE_BINARY_DIR}/${PROJECT_NAME}
            /fo${OUTPUT_RES} "${RC_FILE_PATH}"
        DEPENDS
        ${compiled_shaders} ${SHADER_RC} ${INCLUDE_FILE}
)

add_custom_target(static_res DEPENDS "${OUTPUT_RES}")