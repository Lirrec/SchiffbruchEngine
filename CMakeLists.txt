project(SchiffbruchEngine)

cmake_minimum_required(VERSION 2.8)

option(USE_SHIPPED_SFML2 OFF)

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

find_package(Boost 1.46 COMPONENTS thread system)# regex)
#find_package(Boost 1.46 COMPONENTS thread_win32 system)# W32
find_package(SFML 2 COMPONENTS system window graphics audio)

if(NOT Boost_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
	find_package(Boost REQUIRED thread system) #regex)
endif()

if(NOT SFML_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
	find_package(SFML 2 COMPONENTS system window graphics audio)
	if(NOT SFML_FOUND)
		set(USE_SHIPPED_SFML2 ON)
	endif()
endif()

if(USE_SHIPPED_SFML2)
	message("-- Using shipped sfml2...")
	set(SFML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/sfml2/include)
	if(CMAKE_SYSTEM_NAME MATCHES "Linux")
		link_directories(${PROJECT_SOURCE_DIR}/lib/linux/${CMAKE_SYSTEM_PROCESSOR})
		set(SFML_RPATH "./lib/linux/${CMAKE_SYSTEM_PROCESSOR}:./")
	elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
		link_directories(${PROJECT_SOURCE_DIR}/lib/win/)
		set(SFML_RPATH "./lib/win:./")
	else()
		link_directories(${PROJECT_SOURCE_DIR}/lib/linux/${CMAKE_SYSTEM_PROCESSOR})
		set(SFML_RPATH "./lib/linux/${CMAKE_SYSTEM_PROCESSOR}:./")
	endif()
	set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-audio)
	set(SFML_FOUND TRUE)
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

add_subdirectory(src)
