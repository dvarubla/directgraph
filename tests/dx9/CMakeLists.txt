project(dx9_tests)

set(SHADERS_RES_FILE "${CMAKE_BINARY_DIR}/resources/shaders.res")

add_executable(dx9_bar dx9_bar.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h)
add_executable(dx9_single_pixel dx9_single_pixel.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h)
add_executable(dx9_pixel_container dx9_pixel_container.cpp ${SHADERS_RES_FILE} ../stubs/QueueReaderStub.cpp ../stubs/QueueReaderStub.h common.h)

set_target_properties(
        dx9_bar dx9_single_pixel dx9_pixel_container
        PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
        LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}")

target_link_libraries(dx9_bar PUBLIC directgraph_dx9_int directgraph_testlib)
target_link_libraries(dx9_single_pixel PUBLIC directgraph_dx9_int directgraph_testlib)
target_link_libraries(dx9_pixel_container PUBLIC directgraph_dx9_int directgraph_testlib)
set_source_files_properties(
        "${SHADERS_RES_FILE}"
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)
add_dependencies(dx9_bar static_res)
add_dependencies(dx9_single_pixel static_res)
add_dependencies(dx9_pixel_container static_res)

add_test(NAME dx9_bar WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/previous
        COMMAND $<TARGET_FILE:dx9_bar> .)

add_test(NAME dx9_single_pixel WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/previous
        COMMAND $<TARGET_FILE:dx9_single_pixel> .)

add_test(NAME dx9_pixel_container WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/previous
        COMMAND $<TARGET_FILE:dx9_pixel_container> .)