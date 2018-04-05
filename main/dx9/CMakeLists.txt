project(directgraph_dx9_renderer)

set(DX9_FILES
        Renderer.cpp
        Renderer.h
        WindowFactory.cpp
        WindowFactory.h
        Common.cpp
        Common.h
        Exception.cpp
        Exception.h PatternTexturesHelper.cpp PatternTexturesHelper.h IPatternTexturesHelper.h IFeatures.h DeviceFeatures.cpp DeviceFeatures.h BufferPreparer.cpp BufferPreparer.h ShaderManager.cpp ShaderManager.h primitiveHelpers/EllipseHelper.cpp primitiveHelpers/EllipseHelper.h VertexCreator.h VertexCreator.cpp BufferPreparerParams.cpp BufferPreparerParams.h StateHelper.cpp StateHelper.h PropertyManager.cpp PropertyManager.h DrawItemProcessor.cpp DrawItemProcessor.h DrawOpCreator.cpp DrawOpCreator.h misc.h primitiveHelpers/RectangleHelper.cpp primitiveHelpers/RectangleHelper.h primitiveHelpers/SimplePrimHelper.cpp primitiveHelpers/SimplePrimHelper.h TextureCoordsCalc.cpp TextureCoordsCalc.h drawers/IDrawer.h DrawStateHelper.cpp DrawStateHelper.h DrawerManager.cpp DrawerManager.h drawers/BarDrawer.cpp drawers/BarDrawer.h primitiveHelpers/DegenerateHelper.cpp primitiveHelpers/DegenerateHelper.h drawers/Clearer.cpp drawers/Clearer.h drawers/RectangleDrawer.cpp drawers/RectangleDrawer.h drawers/EllipseDrawer.cpp drawers/EllipseDrawer.h drawers/NullDrawer.cpp drawers/NullDrawer.h drawers/PixelContainerDrawer.cpp drawers/PixelContainerDrawer.h drawers/SinglePixelDrawer.cpp drawers/SinglePixelDrawer.h util.cpp util.h misc.cpp drawers/LineDrawer.cpp drawers/LineDrawer.h primitiveHelpers/LineHelper.cpp primitiveHelpers/LineHelper.h drawers/PolygonDrawer.cpp drawers/PolygonDrawer.h primitiveHelpers/PolygonHelper.cpp primitiveHelpers/PolygonHelper.h)

add_library(directgraph_dx9_renderer ${DX9_FILES})
set_target_properties(
        directgraph_dx9_renderer PROPERTIES
        COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}" LINK_FLAGS "${EXTRA_LINK_FLAGS}"
)
add_dependencies(directgraph_dx9_renderer static_res)

set(LIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include" "." "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/main")

target_link_libraries(directgraph_dx9_renderer d3d9)
target_include_directories(directgraph_dx9_renderer PUBLIC ${LIB_INCLUDE_DIRS})