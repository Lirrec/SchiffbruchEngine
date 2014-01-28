project( SchiffbruchEngine )

cmake_minimum_required( VERSION 2.8 )


#############################################################
################### Cmake Options ###########################
#############################################################


option( PROFILE "" OFF)
option( USE_PYTHON "" OFF)
option( USE_RUBY "" OFF)
option( BUILD_SHARED_LIBS "" ON )
option(ENABLE_LTO "enable lto, if ya dont know what this is, ya dont need it!" OFF)

set( SFGUI_ROOT "" CACHE PATH "Path to where SFGUI is located.")

set( CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH} )
set( CMAKE_MODULE_PATH "${SFGUI_ROOT}/cmake" ${CMAKE_MODULE_PATH} )

SET( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#############################################################
#################### External Libraries Configuration #####################
#############################################################

if (BUILD_SHARED_LIBS)
	set(SFML_STATIC_LIBRARIES false)
	set(SFGUI_STATIC_LIBRARIES false)
else()	
	set(SFML_STATIC_LIBRARIES true)
	set(SFGUI_STATIC_LIBRARIES true)
endif()

#just make sure those two are not empty, as in that case sfgui will add unwanted compile flags
if( NOT CMAKE_CXX_FLAGS )
	set( CMAKE_CXX_FLAGS  " " CACHE STRING "C++ compiler flags" FORCE )
endif()
if( NOT CMAKE_CXX_FLAGS_DEBUG )
	set( CMAKE_CXX_FLAGS_DEBUG  " " CACHE STRING "C++ compiler flags (debug)" FORCE )
endif()

#############################################################
#################### External Libraries #####################
#############################################################

# set external windows and linux libs
#if( CMAKE_SYSTEM_NAME MATCHES Windows )
#  set( GENERIC_LIBS pthread jpeg GLEW freetype openal32 opengl32 sndfile winmm)
#else()
#	set( GENERIC_LIBS pthread jpeg GLEW X11 Xrandr freetype rt openal GL sndfile)
#endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
	set( ADD_SFML_MAIN "main")
endif()

set( SFML_FIND_REQUIRED true )
find_package(Boost 1.46 REQUIRED thread system filesystem)# regex)
find_package(SFML 2 COMPONENTS audio graphics window system ${ADD_SFML_MAIN})
find_package(SFGUI REQUIRED )

include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})

#############################################################
#################### File inclusion #########################
#############################################################

file( GLOB_RECURSE sbe_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file( GLOB_RECURSE sbe_SRCHDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.ipp" )

file( GLOB_RECURSE sbe_HDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/include/*.ipp" )

#left here for debugging purposes
if (true)
	message(STATUS "INFO --")
	message(STATUS "SHARED:\n ${SHARED}")
	message(STATUS "BUILD_SHARED_LIBS:\n ${BUILD_SHARED_LIBS}" )
	message(STATUS "CMAKE_BUILD_TYPE:\n ${CMAKE_BUILD_TYPE}" )
	message(STATUS "INCLUDES --")
	message(STATUS "sfml libs:\n ${SFML_LIBRARIES}" )
	message(STATUS "sfml includes:\n ${SFML_INCLUDE_DIR}" )
	message(STATUS "sfml deps:\n ${SFML_DEPENDENCIES}" )
	message(STATUS "sfguil includes:\n ${SFGUI_LIBRARY}" )
	message(STATUS "sfguil includes:\n ${SFGUI_INCLUDE_DIR}" )
	message(STATUS "boost includes:\n ${Boost_INCLUDE_DIR}" )
	
#	message( "-- FILES --" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
#	message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )
endif()

#############################################################
#################### Compiler Options #######################
#############################################################

if(CMAKE_SYSTEM_NAME MATCHES Windows)

else()
	add_definitions( -fPIC )
endif()

add_definitions( -std=c++11 -DBOOST_THREAD_USE_LIB -pipe )

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DCOUNT_EVENTS)
	add_definitions( -Wall -g -fstack-protector --param=ssp-buffer-size=4 )
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	if( PROFILE )
		add_definitions( -O1 )
	else()
		add_definitions( -fomit-frame-pointer -fexpensive-optimizations -O3 )
	endif()

endif()

if( PROFILE )
	add_definitions( -pg )
endif()
if(ENABLE_LTO)
	add_definitions( -flto )
endif()


#link_directories(${SFML_ROOT})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

#############################################################
#################### Scripting / Swig #######################
#############################################################

add_subdirectory( scripting )
# set by the scripting CMakelists
include_directories( ${SBE_SCRIPT_INCLUDES} )

#################### Final lib ##############################

add_library(sbe ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})

if( BUILD_SHARED_LIBS )
	add_definitions(-Wl,-rdynamic)
endif()


target_link_libraries(sbe ${SFGUI_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${SBE_SCRIPT_LIBS} ${SFML_DEPENDENCIES} ${GENERIC_LIBS})
