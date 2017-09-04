project(directgraph)

set(DX9_FILES
    dx9/DX9Renderer.cpp
    dx9/DX9Renderer.h
    dx9/DX9WindowFactory.cpp
    dx9/DX9WindowFactory.h
        dx9/DX9Common.cpp dx9/DX9Common.h dx9/DX9Exception.cpp dx9/DX9Exception.h)

set(D2D_FILES
    d2d/D2DRenderer.cpp
    d2d/D2DRenderer.h
    d2d/BufferCreator.cpp
    d2d/BufferCreator.h
    d2d/D2DWindowFactory.cpp
    d2d/D2DWindowFactory.h
        d2d/D2DCommon.cpp d2d/D2DCommon.h)

set(COMMON_FILES
    IRenderer.h
    common.h
    DPIHelper.cpp
    DPIHelper.h
    MyWindow.cpp
    MyWindow.h
    directgraph_mainloop.cpp
    resource.cpp
    resource.h
        WException.cpp WException.h IWindowFactory.h DPIConverter.cpp DPIConverter.h
)

set(INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/directgraph_mainloop.h
)

include(GenerateExportHeader)

set(LIB_DX9_FILES ${INCLUDE_FILES} ${COMMON_FILES} ${DX9_FILES})

set(LIB_D2D_FILES ${INCLUDE_FILES} ${COMMON_FILES} ${D2D_FILES})

set(LIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include" "." "${CMAKE_BINARY_DIR}")

add_library(directgraph_dx9 STATIC ${LIB_DX9_FILES})
add_library(directgraph_d2d STATIC ${LIB_D2D_FILES})

target_include_directories(directgraph_dx9 BEFORE PRIVATE ${LIB_INCLUDE_DIRS})
target_include_directories(directgraph_d2d BEFORE PRIVATE ${LIB_INCLUDE_DIRS})
target_link_libraries(directgraph_dx9 d3d9)
target_link_libraries(directgraph_d2d d2d1)
set_target_properties(directgraph_dx9 directgraph_d2d PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)

add_library(directgraph INTERFACE)
target_link_libraries(directgraph INTERFACE directgraph_dx9 directgraph_d2d)