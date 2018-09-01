project( SchiffbruchEngine )

option( SBE_ENABLE_COTIRE "" OFF)

if(SBE_ENABLE_COTIRE)
	include(cotire)
endif()
cmake_minimum_required( VERSION 3.8 )

# set sbe version
set(VER_MAJOR 0)
set(VER_MINOR 1)

#############################################################
################### Cmake Options ###########################
#############################################################

option( SBE_PROFILE "" OFF)
option( SBE_USE_PYTHON "" OFF)
option( SBE_USE_RUBY "" OFF)
option( SBE_BUILD_SHARED_LIBS "" ON )
option( SBE_USE_SHARED_LIBS "" ON )
option( SBE_ENABLE_LTO "enable ltoclion " OFF)

SET( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "-- INFO --")
message(STATUS "SBE_BUILD_SHARED_LIBS: ${SBE_BUILD_SHARED_LIBS}" )
message(STATUS "CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}" )
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}" )
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}\n")

#############################################################
#################### External Libraries Configuration #####################
#############################################################

if (SBE_USE_SHARED_LIBS)
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
find_package(Boost 1.60 COMPONENTS thread system filesystem)
#find_package(SFML 2 COMPONENTS audio graphics window system main ${ADD_SFML_MAIN})
find_package(SFML 2 COMPONENTS audio window graphics system ${ADD_SFML_MAIN} REQUIRED)
find_package(SFGUI REQUIRED )
find_package(glm REQUIRED )

add_definitions( -DGLM_FORCE_RADIANS -DGLM_FORCE_CXX11 -DGLM_ENABLE_EXPERIMENTAL )

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


	message(STATUS "-- INCLUDES --")
	message(STATUS "sfml libs:      ${SFML_LIBRARIES}" )
	message(STATUS "sfml includes:  ${SFML_INCLUDE_DIR}" )
	message(STATUS "sfml deps:      ${SFML_DEPENDENCIES}" )
	message(STATUS "sfgui lib:      ${SFGUI_LIBRARY}" )
	message(STATUS "sfgui includes: ${SFGUI_INCLUDE_DIR}" )
	message(STATUS "boost includes: ${Boost_INCLUDE_DIR}\n\n" )

	message(STATUS "-- CXX FEATURES SUPPORTED BY COMPILER --")
	message(STATUS "compile features: ${CMAKE_CXX_COMPILE_FEATURES}\n\n" )

#	message( "-- FILES --" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
#	message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
#	message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )
endif()

#############################################################
#################### Compiler Options #######################
#############################################################

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME MATCHES Windows)
	# nogdi prevents a "#define ERROR 0" from wingdi.h which fucks up our code
	# wingdi.h is sometimes included in gl.h ( some mingw versions of glew don't include it, most do )
	add_definitions( -DNOGDI -DWIN32_LEAN_AND_MEAN )

	#disable boosts auto-linking feature. The required libs are already included by find_package
	add_definitions(-DBOOST_ALL_NO_LIB)

	# enable dllexport define in config.h
	set(DLLEXPORT "__declspec(dllexport)")

else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

#
#	General Compiler Options
#
IF(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_THREAD_USE_LIB -D_stdcall -D_cdecl -D_thiscall -D_fastcall -pipe")

else()
	# FIXME: remove std-flag here if CMAKE_CXX_STANDARD works with msvc!
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /MP /Gm- /EHsc /GR /W3")
endif()

#
#	Debug Options
#
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCOUNT_EVENTS")
if(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fstack-protector --param=ssp-buffer-size=4")
else()
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /RTC1")
endif()

#
#	Release Options
#

if( SBE_PROFILE)
	if (MSVC)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /PROFILE")
		add_definitions()
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1 -pg")
	endif()
else()
	if (MSVC)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Oy")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -fexpensive-optimizations")
	endif()
endif()


if(SBE_ENABLE_LTO AND NOT MSVC)
	if (MSVC)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
	endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

#############################################################
#################### Scripting / Swig #######################
#############################################################

add_subdirectory( scripting )
# set by the scripting CMakelists
include_directories( ${SBE_SCRIPT_INCLUDES} )

#################### Generate config.h ######################

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#################### Final lib ##############################
# Add the library.
if( SBE_BUILD_SHARED_LIBS )

	#set_target_properties( sbe PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE )
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

	add_library( sbe SHARED ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS} )

	set_target_properties( sbe PROPERTIES DEBUG_POSTFIX -d )




	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_SYSTEM_NAME MATCHES Windows) # mingw-gcc!
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -Wl,-rdynamic, -Wl,-export-all-symbols")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default")
	else() # msvc
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /LD")

	endif()
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSBE_STATIC")
	add_library( sbe ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS} include/sbe/util/Meta.hpp)

	set_target_properties( sbe PROPERTIES DEBUG_POSTFIX -s-d )

	set_target_properties( sbe PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( sbe PROPERTIES RELEASE_POSTFIX -s )
	set_target_properties( sbe PROPERTIES MINSIZEREL_POSTFIX -s )
endif()


#
# setup of the library
#

target_link_libraries(sbe ${SFGUI_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${SBE_SCRIPT_LIBS} ${SFML_DEPENDENCIES})

set_target_properties(sbe PROPERTIES SOVERSION ${VER_MAJOR})
set_target_properties(sbe PROPERTIES VERSION ${VER_MAJOR}.${VER_MINOR})


#set_target_properties(sbe PROPERTIES PUBLIC_HEADER "${sbe_HDRS}")

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS sbe
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include)

set(CM_VER ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
install(FILES cmake/FindSBE.cmake DESTINATION share/cmake-${CM_VER}/Modules)



if(SBE_ENABLE_COTIRE)
	set_target_properties(sbe PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER FALSE)
	cotire(sbe)
endif()
