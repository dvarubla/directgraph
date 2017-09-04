project(tests)

set(SHADERS_RES_FILE "${CMAKE_BINARY_DIR}/resources/shaders.res")

add_executable(dx9_bar dx9/dx9_bar.cpp ${SHADERS_RES_FILE})

set(TESTS_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/main" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}")

target_include_directories(dx9_bar BEFORE PUBLIC ${TESTS_INCLUDE_DIRS})
set_target_properties(dx9_bar PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
        LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}")

target_link_libraries(dx9_bar directgraph_dx9 directgraph_testlib)
set_source_files_properties(
        "${SHADERS_RES_FILE}"
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)
add_dependencies(dx9_bar static_res)

add_executable(d2d_bar d2d/d2d_bar.cpp)
target_include_directories(d2d_bar BEFORE PUBLIC ${TESTS_INCLUDE_DIRS})
set_target_properties(d2d_bar PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
        LINK_FLAGS "/ENTRY:wmainCRTStartup /subsystem:console ${EXTRA_LINK_FLAGS}")

target_link_libraries(d2d_bar directgraph_d2d directgraph_testlib)