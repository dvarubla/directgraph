project(directgraph)

add_subdirectory(dx9)

set(COMMON_FILES
    QueueItem.h
    IMyWindow.h util.cpp util.h PixelContainerCreator.cpp PixelContainerCreator.h
    IRenderer.h
    common.h
    DPIHelper.cpp
    DPIHelper.h
    MyWindow.cpp
    MyWindow.h
    resource.cpp
    resource.h
        WindowManager.cpp WindowManager.h IController.h IWindowListener.h
        WException.cpp WException.h IWindowFactory.h DPIConverter.cpp DPIConverter.h
        Queue.cpp Queue.h ThreadController.cpp ThreadController.h QueueReader.cpp QueueReader.h CommonProps.cpp CommonProps.h ../include/directgraph/directgraph_constants.h IPixelContainer.h PixelContainer.cpp PixelContainer.h NullWindowListener.cpp NullWindowListener.h RWRPLock.cpp RWRPLock.h ControllerFactory.cpp ControllerFactory.h IControllerFactory.h QueueItem.cpp WinManException.cpp WinManException.h error_handler.cpp error_handler.h FormatFinder.h patterns.h patterns.cpp)

set(INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/directgraph/directgraph_api.h
    ${CMAKE_SOURCE_DIR}/include/directgraph/export_header.h
    ${CMAKE_SOURCE_DIR}/include/directgraph/directgraph_stdint.h
)

set(API_FILES directgraph_api.cpp)

set(COMMON_LIB_FILES ${INCLUDE_FILES} ${COMMON_FILES})
add_library(directgraph_common ${COMMON_LIB_FILES})
set_target_properties(
        directgraph_common PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)
target_include_directories(directgraph_common BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/include")

add_library(directgraph_dx9_int INTERFACE)
target_link_libraries(directgraph_dx9_int INTERFACE directgraph_common directgraph_dx9_renderer)

add_library(directgraph_dx9 SHARED ${API_FILES})
set_target_properties(
        directgraph_dx9 PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)
target_link_libraries(directgraph_dx9 directgraph_dx9_int)
target_compile_definitions(directgraph_dx9 PUBLIC DIRECTGRAPH_EXPORTS)

add_custom_command(TARGET directgraph_dx9
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:directgraph_dx9>/directgraph_dx9.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:directgraph_dx9>/directgraph_dx9.lib ${CMAKE_BINARY_DIR}
)
