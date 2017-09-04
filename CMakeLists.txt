cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE 1)

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness 64)
endif()

option(XPCOMPAT "Enable compatibility with XP" OFF)

set(COMMON_COMPILE_FLAGS "/MTd /D_UNICODE /DUNICODE /WX")

set(EXTRA_LINK_FLAGS)
if(XPCOMPAT)
    message("Compatibility with XP enabled")
    set(EXTRA_LINK_FLAGS "/SUBSYSTEM:CONSOLE,5.01")
endif()

add_subdirectory(resources)
add_subdirectory(main)
add_subdirectory(testlib)
add_subdirectory(tests)