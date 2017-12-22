project(directgraph)

add_subdirectory(dx9)

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
        Queue.cpp Queue.h ThreadController.cpp ThreadController.h QueueReader.cpp QueueReader.h CommonProps.cpp CommonProps.h ../include/graphics_const.h graphics_const_internal.h IPixelContainer.h PixelContainer.cpp PixelContainer.h)

set(INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/directgraph_mainloop.h
)

include(GenerateExportHeader)

set(COMMON_LIB_FILES ${INCLUDE_FILES} ${COMMON_FILES} IMyWindow.h)
add_library(directgraph_common ${COMMON_LIB_FILES})
set_target_properties(
        directgraph_common PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)
target_include_directories(directgraph_common BEFORE PUBLIC . "${CMAKE_SOURCE_DIR}/include")

add_library(directgraph_dx9 INTERFACE)
target_link_libraries(directgraph_dx9 INTERFACE directgraph_common directgraph_dx9_renderer)
