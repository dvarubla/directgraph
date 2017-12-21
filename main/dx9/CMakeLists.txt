project(directgraph_dx9_renderer)

set(DX9_FILES
        DX9Renderer.cpp
        DX9Renderer.h
        DX9WindowFactory.cpp
        DX9WindowFactory.h
        DX9Common.cpp
        DX9Common.h
        DX9Exception.cpp
        DX9Exception.h)

add_library(directgraph_dx9_renderer ${DX9_FILES})
set_target_properties(
        directgraph_dx9_renderer PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)

set(LIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include" "." "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/main")

target_link_libraries(directgraph_dx9_renderer d3d9)
target_include_directories(directgraph_dx9_renderer PUBLIC ${LIB_INCLUDE_DIRS})