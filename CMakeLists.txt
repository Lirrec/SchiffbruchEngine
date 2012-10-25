project(SchiffbruchEngine)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})

option(USE_CUSTOM_BOOST "" OFF)
option(USE_CUSTOM_SFML "" ON)
set(CUSTOM_BOOST_ROOT "" CACHE PATH "custom boost build root dir")
set(CUSTOM_SFML_ROOT "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML" CACHE PATH "custom SFML build root dir")

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if(USE_CUSTOM_BOOST)
	message("-- Using custom boost...")
	set(BOOST_ROOT "${CUSTOM_BOOST_ROOT}")
	set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
	set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
endif()

if(USE_CUSTOM_SFML)
	message("-- Using custom SFML...")
	set(SFML_ROOT "${CUSTOM_SFML_ROOT}/build")
	set(SFML_INCLUDE_DIR "${CUSTOM_SFML_ROOT}/include")
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
	if(NOT SFML_FOUND)
		message( FATAL_ERROR "Please build SFML in '${CUSTOM_SFML_ROOT}/build' first!")
	endif()
endif()

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
endif()
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
endif()

#option(SHARED OFF)

# fix mingw + boost
if(CMAKE_SYSTEM_NAME MATCHES Windows)
	add_definitions(-std=gnu++0x)
else()
	add_definitions(-std=c++0x)
endif()

add_definitions(-Wall -DCOUNT_EVENTS -fPIC)

include_directories(${PROJECT_SOURCE_DIR}/include) # needed?
link_directories(${SFML_ROOT})
#link_directories("/usr/lib")

add_subdirectory(SFGUI)
add_subdirectory(src)