project(directgraph_testlib)

set(LIB_FILES testlib.cpp testlib_funcs.cpp testlib_funcs.h ImageTester.cpp ImageTester.h BitmapWrap.cpp BitmapWrap.h)

add_library(directgraph_testlib STATIC ${LIB_FILES})

enable_testing()
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/gtest")
find_package(GTest REQUIRED)

target_link_libraries(directgraph_testlib GTest::GTest)

target_include_directories(directgraph_testlib BEFORE PRIVATE ".")
target_link_libraries(directgraph_testlib gdiplus)
set_target_properties(directgraph_testlib PROPERTIES
    COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}"
)