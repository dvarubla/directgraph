cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE 1)

if(COMMAND start_hook)
start_hook()
endif()

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness 64)
endif()

option(XPCOMPAT "Enable compatibility with XP" OFF)
option(XP64COMPAT "Enable compatibility with XP x64" OFF)
option(IA32COMPAT "Don't use enhanced instructions" OFF)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} ${DEBUG_COMPILE_FLAGS}")
    set(EXTRA_LINK_FLAGS "${EXTRA_LINK_FLAGS} ${DEBUG_LINK_FLAGS}")
else()
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} ${RELEASE_COMPILE_FLAGS}")
    set(EXTRA_LINK_FLAGS "${EXTRA_LINK_FLAGS} ${RELEASE_LINK_FLAGS}")
endif()

if(IA32COMPAT)
    message("Enhanced instructions are not used")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} ${IA32COMPAT_COMPILE_FLAGS}")
endif()


if(XPCOMPAT)
    message("Compatibility with XP enabled")
    set(EXTRA_LINK_FLAGS "${EXTRA_LINK_FLAGS} ${XPCOMPAT_COMPILE_FLAGS}")
elseif(XP64COMPAT)
    message("Compatibility with XP x64 enabled")
    set(EXTRA_LINK_FLAGS "${EXTRA_LINK_FLAGS} ${XP64COMPAT_COMPILE_FLAGS}")
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
set(CMAKE_DEBUG_POSTFIX "")

enable_testing()

set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS}")
add_definitions(${COMMON_DEFS})
add_subdirectory("gtest/googlemock" EXCLUDE_FROM_ALL)
set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}")

add_subdirectory(resources)
add_subdirectory(main)
add_subdirectory(testlib)
add_subdirectory(tests)