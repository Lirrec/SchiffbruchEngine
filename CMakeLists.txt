project( SchiffbruchEngine )

cmake_minimum_required( VERSION 2.8 )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML/cmake/Modules" ${CMAKE_MODULE_PATH} )


#############################################################
################### Cmake Options ###########################
#############################################################


option( PROFILE "" OFF)
option( USE_CUSTOM_BOOST "" OFF)
option( USE_CUSTOM_SFML "" ON)
set( CUSTOM_BOOST_ROOT "" CACHE PATH "custom boost build root dir")
set( CUSTOM_SFML_ROOT "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML" CACHE PATH "custom SFML build root dir")


SET( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE "Release")
endif()
message( "-- CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#############################################################
#################### External Libraries #####################
#############################################################

# set external windows and linux libs
if( CMAKE_SYSTEM_NAME MATCHES Windows )
  set( GENERIC_LIBS pthread jpeg GLEW freetype openal32 sndfile winmm)
else()
  set( GENERIC_LIBS pthread jpeg GLEW X11 Xrandr freetype rt openal sndfile)
endif()

if( USE_CUSTOM_BOOST)
	message("-- Using custom boost...")
	set(BOOST_ROOT "${CUSTOM_BOOST_ROOT}")
endif()

if( USE_CUSTOM_SFML)
	message("-- Using custom SFML...")
	set(SFML_ROOT "${CUSTOM_SFML_ROOT}/build")
	set(SFML_INCLUDE_DIR "${CUSTOM_SFML_ROOT}/include")
endif(USE_CUSTOM_SFML)

find_package(Boost 1.46 REQUIRED thread system filesystem)# regex)

#only audio required, the others are pulled in by sfgui
find_package(SFML 2 REQUIRED audio)
#find_package(SFML 2 COMPONENTS graphics window system audio)

foreach(LIB ${GENERIC_LIBS})
	find_library(FOUND_${LIB} ${LIB} )
endforeach()

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

########################################################
######################## SFGUI #########################
########################################################

set( SFGUI_BUILD_EXAMPLES false CACHE BOOL "Do or dont build examples)")
set( BUILD_SHARED_LIBS false CACHE BOOL "Build dynamic library." )
set( SFML_STATIC_LIBRARIES true CACHE BOOL "Was SFML built as a static library?" )

#just make sure those two are not empty, as in that case sfgui will add unwanted compile flags
if( NOT CMAKE_CXX_FLAGS )
	set( CMAKE_CXX_FLAGS  " " CACHE STRING "C++ compiler flags" FORCE )
endif()
if( NOT CMAKE_CXX_FLAGS_DEBUG )
	set( CMAKE_CXX_FLAGS_DEBUG  " " CACHE STRING "C++ compiler flags (debug)" FORCE )
endif()

add_subdirectory(SFGUI)

########################################################
############## Check for Boost and SFML ################
########################################################

if(NOT Boost_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
else()
	include_directories(${Boost_INCLUDE_DIR})
endif()

if(NOT SFML_FOUND)
	message( FATAL_ERROR "Please build SFML in '${CUSTOM_SFML_ROOT}/build' first!")
else()
	include_directories(${SFML_INCLUDE_DIR})
endif()

########################################################
############ compiler version detection ################
########################################################

if (CMAKE_COMPILER_IS_GNUCXX  AND NOT VERSION_CHECK_DONE)
	# get gcc compiler version if not set
	# if newer cmake, CMAKE_CXX_COMPILER_VERSION is set, so use it...
	# if not, call 'c++ -dumpversion' and get it from there
	if(CMAKE_CXX_COMPILER_VERSION)
		set(GCC_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
	else()
		execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	endif()
endif()

if(NOT VERSION_CHECK_DONE)
	# set some definitions depending on gcc version
	if (GCC_VERSION VERSION_EQUAL 4.7 OR GCC_VERSION VERSION_GREATER 4.7)
	    add_definitions ( -std=c++11 )

			if(GCC_VERSION VERSION_EQUAL 4.8 OR GCC_VERSION VERSION_GREATER 4.8)
				add_definitions ( -D__GCC_4_8__ )
				if(CMAKE_BUILD_TYPE MATCHES Debug)
					add_definitions( -Og )
				endif()
			endif()
	else()
		if(GCC_VERSION VERSION_EQUAL 4.6 OR GCC_VERSION VERSION_GREATER 4.6)
	    add_definitions ( -std=c++0x -D__GCC_4_6__ )
		else()
			message("-- Unsupported compiler detected. Build might fail!")
			# changing this to -std=c++0x if you've got an old compiler, might work
			add_definitions( -std=c++11 )
		endif()
	endif()
	set(VERSION_CHECK_DONE ON)
endif()



#############################################################
#################### File inclusion #########################
#############################################################

file( GLOB_RECURSE sbe_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file( GLOB_RECURSE sbe_SRCHDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.ipp" )

file( GLOB_RECURSE sbe_HDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/include/*.ipp" )

#left here for debugging purposes
if (false)
	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
	message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
	message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )
endif()

#############################################################
#################### Compiler Options #######################
#############################################################

add_definitions( -DBOOST_THREAD_USE_LIB )

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DCOUNT_EVENTS)
	add_definitions( -Wall -g )
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

link_directories(${SFML_ROOT})

if(SHARED)
    add_library(sbe SHARED ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
else()
    add_library(sbe STATIC ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
    add_definitions(-DSFML_STATIC -DSFGUI_STATIC)
endif()

set_property(GLOBAL PROPERTY SBE_LINK_LIBS "${Boost_LIBRARIES};${SFML_LIBRARIES};${GENERIC_LIBS}")
set_property(GLOBAL PROPERTY SBE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include;${PROJECT_SOURCE_DIR}/SFGUI/include;${Boost_INCLUDE_DIR};${SFML_INCLUDE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/SFGUI/include)

target_link_libraries(sbe sfgui ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${GENERIC_LIBS})

#############################################################
#################### Scripting / Swig #######################
#############################################################

add_subdirectory( scripting )

