cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE 1)

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness 64)
endif()

option(XPCOMPAT "Enable compatibility with XP" OFF)
option(XP64COMPAT "Enable compatibility with XP x64" OFF)
option(IA32COMPAT "Don't use enhanced instructions" OFF)

set(COMMON_COMPILE_FLAGS "/D_UNICODE /DUNICODE /WX")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} /MTd")
else()
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} /MT")
endif()

if(IA32COMPAT)
    message("Enhanced instructions are not used")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} /arch:IA32")
endif()

set(EXTRA_LINK_FLAGS)
if(XPCOMPAT)
    message("Compatibility with XP enabled")
    set(EXTRA_LINK_FLAGS "/SUBSYSTEM:CONSOLE,5.01")
elseif(XP64COMPAT)
    message("Compatibility with XP x64 enabled")
    set(EXTRA_LINK_FLAGS "/SUBSYSTEM:CONSOLE,5.02")
endif()

set(CMAKE_DEBUG_POSTFIX "")

enable_testing()

set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS}")
add_definitions(/D_VARIADIC_MAX=10 /D_SCL_SECURE_NO_WARNINGS)
add_subdirectory("gtest/googlemock" EXCLUDE_FROM_ALL)
set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}")

add_subdirectory(resources)
add_subdirectory(main)
add_subdirectory(testlib)
add_subdirectory(tests)