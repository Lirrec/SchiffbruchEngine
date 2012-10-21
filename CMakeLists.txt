project(SchiffbruchEngine)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})

option(USE_CUSTOM_BOOST "" OFF)
set(CUSTOM_BOOST_ROOT "" CACHE STRING "custom boost build root dir")
option(USE_CUSTOM_SFML "" ON)
set(CUSTOM_SFML_ROOT "${CMAKE_SOURCE_DIR}/libsbe/SFGUI/extlibs/SFML" CACHE STRING "custom SFML build root dir")

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if(USE_CUSTOM_BOOST)
	message("-- Using custom boost...")
	set(BOOST_ROOT "${CUSTOM_BOOST_ROOT}")
	set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
	set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
endif()

if(USE_CUSTOM_SFML)
	message("-- Using custom SFML...")
	set(SFML_ROOT "${CUSTOM_SFML_ROOT}")
	set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
endif(USE_CUSTOM_SFML)

find_package(Boost 1.46 COMPONENTS thread system)# regex)
#find_package(Boost 1.46 COMPONENTS thread_win32 system)# W32
find_package(SFML 2 COMPONENTS system window graphics audio)

if(NOT Boost_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
	find_package(Boost REQUIRED thread system) #regex)
endif()

if(NOT SFML_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
	find_package(SFML 2 COMPONENTS system window graphics audio)
endif()

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
endif()
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
endif()
#find_package(SFML 1.6 REQUIRED system window graphics audio)

option(SHARED OFF)

# fix mingw + boost
if(CMAKE_SYSTEM_NAME MATCHES Windows)
	add_definitions(-std=gnu++0x)
else()
	add_definitions(-std=c++0x)
endif()

add_definitions(-Wall -DCOUNT_EVENTS -fPIC)

include_directories(${PROJECT_SOURCE_DIR}/include) # needed?

add_subdirectory(SFGUI)
add_subdirectory(src)
