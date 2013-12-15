project( SchiffbruchEngine )

cmake_minimum_required( VERSION 2.8 )


#############################################################
################### Cmake Options ###########################
#############################################################


option( PROFILE "" OFF)
option( USE_PYTHON "" OFF)
option( USE_RUBY "" OFF)
option( BUILD_SHARED_LIBS "" ON)
option(ENABLE_LTO "enable lto, if ya dont know what this is, ya dont need it!" OFF)

set( CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH} )
SET( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#############################################################
#################### External Libraries Configuration #####################
#############################################################

set( SFGUI_BUILD_EXAMPLES false CACHE BOOL "Do or dont build examples)")
if ( ${BUILD_SHARED_LIBS})
	set( SFML_STATIC_LIBRARIES false)
else()
	set( SFML_STATIC_LIBRARIES true)
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
if( CMAKE_SYSTEM_NAME MATCHES Windows )
  set( GENERIC_LIBS pthread jpeg GLEW freetype openal32 opengl32 sndfile winmm)
else()
	set( GENERIC_LIBS pthread jpeg GLEW X11 Xrandr freetype rt openal GL sndfile)
endif()

set( SFML_FIND_REQUIRED true )
find_package(Boost 1.46 REQUIRED thread system filesystem)# regex)
find_package(SFML 2 COMPONENTS audio graphics window system)
find_package( SFGUI REQUIRED )

include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})

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
				if(GCC_VERSION VERSION_EQUAL 4.9 OR GCC_VERSION VERSION_GREATER 4.9)
					add_definitions( -D__GCC_4_9__ )
				else()
					add_definitions ( -D__GCC_4_8__ )
				endif()

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
	message( "-- INFO --")
	message( "-- SHARED:\n ${SHARED}")
	message( "-- BUILD_SHARED_LIBS:\n ${BUILD_SHARED_LIBS}" )
	message( "-- CMAKE_BUILD_TYPE:\n ${CMAKE_BUILD_TYPE}" )
	message( "-- INCLUDES --")
	message( "-- sfml libs:\n ${SFML_LIBRARIES}" )
	message( "-- sfml includes:\n ${SFML_INCLUDE_DIR}" )
	message( "-- sfml deps:\n ${SFML_DEPENDENCIES}" )
	message( "-- sfguil includes:\n ${SFGUI_LIBRARY}" )
	message( "-- sfguil includes:\n ${SFGUI_INCLUDE_DIR}" )
	message( "-- boost includes:\n ${Boost_INCLUDE_DIR}" )
	
#	message( "-- FILES --" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
#	message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )
endif()



#############################################################
#################### Compiler Options #######################
#############################################################

add_definitions( -fPIC -DBOOST_THREAD_USE_LIB -pipe )

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


link_directories(${SFML_ROOT})

set_property(GLOBAL PROPERTY SBE_LINK_LIBS "${Boost_LIBRARIES};${SFGUI_LIBRARY};${SFML_LIBRARIES};${GENERIC_LIBS}")
set_property(GLOBAL PROPERTY SBE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include;${SFGUI_INCLUDE_DIR};${Boost_INCLUDE_DIR};${SFML_INCLUDE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

#############################################################
#################### Scripting / Swig #######################
#############################################################

add_subdirectory( scripting )
# set by the scripting CMakelists
include_directories( ${SBE_SCRIPT_INCLUDES} )

#################### Final lib ##############################

if(BUILD_SHARED_LIBS)
	add_definitions(-Wl,-rdynamic)
endif()

add_library(sbe ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
target_link_libraries(sbe ${SFGUI_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${SBE_SCRIPT_LIBS} ${GENERIC_LIBS})
