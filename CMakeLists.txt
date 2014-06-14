project( SchiffbruchEngine )

cmake_minimum_required( VERSION 2.8 )

# set sbe version
set(VER_MAJOR 0)
set(VER_MINOR 1)

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
	set( ADD_SFML_MAIN "main")
endif()

#############################################################
#################### External Libraries #####################
#############################################################

set( SFML_FIND_REQUIRED true )
find_package(Boost 1.46 REQUIRED thread system filesystem)
find_package(SFML 2 COMPONENTS audio graphics window system ${ADD_SFML_MAIN})
find_package(SFGUI REQUIRED )
find_package(GLM REQUIRED )

include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

#############################################################
#################### File inclusion #########################
#############################################################

file( GLOB_RECURSE sbe_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file( GLOB_RECURSE sbe_SRCHDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.ipp" )

file( GLOB_RECURSE sbe_HDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/include/*.ipp" )

#left here for debugging purposes
if (true)
	message(STATUS "INFO --")
	message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}" )
	message(STATUS "CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}" )
	message(STATUS "INCLUDES --")
	message(STATUS "sfml libs:      ${SFML_LIBRARIES}" )
	message(STATUS "sfml includes:  ${SFML_INCLUDE_DIR}" )
	message(STATUS "sfml deps:      ${SFML_DEPENDENCIES}" )
	message(STATUS "sfgui lib:      ${SFGUI_LIBRARY}" )
	message(STATUS "sfgui includes: ${SFGUI_INCLUDE_DIR}" )
	message(STATUS "boost includes: ${Boost_INCLUDE_DIR}" )

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
	add_definitions( -Wall -g )
	if(NOT CMAKE_SYSTEM_NAME MATCHES Windows)
		add_definitions( -fstack-protector --param=ssp-buffer-size=4 )
	endif()

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

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

#############################################################
#################### Scripting / Swig #######################
#############################################################

add_subdirectory( scripting )
# set by the scripting CMakelists
include_directories( ${SBE_SCRIPT_INCLUDES} )

#################### Final lib ##############################
# Add the library.
if( BUILD_SHARED_LIBS )
	add_library( sbe SHARED ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS} )

	set_target_properties( sbe PROPERTIES DEBUG_POSTFIX -d )

	add_definitions(-Wl,-rdynamic)
else()
	add_definitions( -DSBE_STATIC )
	add_library( sbe ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS} )

	set_target_properties( sbe PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( sbe PROPERTIES RELEASE_POSTFIX -s )
	set_target_properties( sbe PROPERTIES MINSIZEREL_POSTFIX -s )
endif()

target_link_libraries(sbe ${SFGUI_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${SBE_SCRIPT_LIBS} ${SFML_DEPENDENCIES})

set_target_properties(sbe PROPERTIES SOVERSION ${VER_MAJOR})
set_target_properties(sbe PROPERTIES VERSION ${VER_MAJOR}.${VER_MINOR})

install(DIRECTORY include DESTINATION . COMPONENT devel)

install(TARGETS sbe
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib )
install(FILES cmake/FindSBE.cmake DESTINATION share/cmake-2.8/Modules)


