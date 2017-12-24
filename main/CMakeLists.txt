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
    IMyWindow.h util.cpp util.h PixelContainerFactory.cpp PixelContainerFactory.h
    IRenderer.h
    common.h
    DPIHelper.cpp
    DPIHelper.h
    MyWindow.cpp
    MyWindow.h
    resource.cpp
    resource.h
        WException.cpp WException.h IWindowFactory.h DPIConverter.cpp DPIConverter.h
        Queue.cpp Queue.h ThreadController.cpp ThreadController.h QueueReader.cpp QueueReader.h CommonProps.cpp CommonProps.h ../include/directgraph/directgraph_constants.h graphics_const_internal.h IPixelContainer.h PixelContainer.cpp PixelContainer.h)

set(INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/directgraph/directgraph_api.h
    ${CMAKE_SOURCE_DIR}/include/directgraph/export_header.h
)

set(API_FILES directgraph_api.cpp)

set(COMMON_LIB_FILES ${INCLUDE_FILES} ${COMMON_FILES})
add_library(directgraph_common ${COMMON_LIB_FILES})
set_target_properties(
        directgraph_common PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)
target_include_directories(directgraph_common BEFORE PUBLIC . "${CMAKE_SOURCE_DIR}/include")

add_library(directgraph_dx9_int INTERFACE)
target_link_libraries(directgraph_dx9_int INTERFACE directgraph_common directgraph_dx9_renderer)

add_library(directgraph_dx9 SHARED ${API_FILES})
set_target_properties(
        directgraph_dx9 PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)
target_link_libraries(directgraph_dx9 directgraph_common directgraph_dx9_renderer)
target_compile_definitions(directgraph_dx9 PUBLIC DIRECTGRAPH_EXPORTS)
