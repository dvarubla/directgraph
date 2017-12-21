project(directgraph_testlib)

set(LIB_FILES testlib.cpp testlib_funcs.cpp testlib_funcs.h ImageTester.cpp ImageTester.h BitmapWrap.cpp BitmapWrap.h)

add_library(directgraph_testlib STATIC ${LIB_FILES})
add_library(directgraph_gtest_lib INTERFACE)

enable_testing()
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/gtest")
find_package(GTest REQUIRED)

target_link_libraries(directgraph_gtest_lib INTERFACE GTest::GTest)

set(TESTS_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/main" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}")

target_include_directories(directgraph_gtest_lib INTERFACE ${TESTS_INCLUDE_DIRS})

target_include_directories(directgraph_testlib BEFORE PRIVATE ".")
target_link_libraries(directgraph_testlib PUBLIC directgraph_gtest_lib gdiplus)
set_target_properties(directgraph_testlib PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)