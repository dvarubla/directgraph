project(directgraph_dx9_renderer)

set(DX9_FILES
        Renderer.cpp
        Renderer.h
        WindowFactory.cpp
        WindowFactory.h
        Common.cpp
        Common.h
        Exception.cpp
        Exception.h PatternTexturesHelper.cpp PatternTexturesHelper.h IPatternTexturesHelper.h IFeatures.h DeviceFeatures.cpp DeviceFeatures.h PrimitiveCreator.cpp PrimitiveCreator.h BufferPreparer.cpp BufferPreparer.h ShaderManager.cpp ShaderManager.h EllipseHelper.cpp EllipseHelper.h VertexCreator.h VertexCreator.cpp BufferPreparerParams.cpp BufferPreparerParams.h StateHelper.cpp StateHelper.h PropertyManager.cpp PropertyManager.h DrawItemProcessor.cpp DrawItemProcessor.h DrawOpCreator.cpp DrawOpCreator.h DrawStateProcessor.cpp DrawStateProcessor.h misc.h RectangleHelper.cpp RectangleHelper.h SimplePrimHelper.cpp SimplePrimHelper.h)

add_library(directgraph_dx9_renderer ${DX9_FILES})
set_target_properties(
        directgraph_dx9_renderer PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}" LINK_FLAGS "${EXTRA_LINK_FLAGS}"
)
add_dependencies(directgraph_dx9_renderer static_res)

set(LIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include" "." "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/main")

target_link_libraries(directgraph_dx9_renderer d3d9)
target_include_directories(directgraph_dx9_renderer PUBLIC ${LIB_INCLUDE_DIRS})