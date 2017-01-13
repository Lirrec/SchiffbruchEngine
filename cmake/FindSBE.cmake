# This script locates the SBE library.
#
# USAGE
#
# By default, the dynamic version of SBE will be found. To find the static
# one instead, you must set the SBE_STATIC_LIBRARIES variable to TRUE before
# calling find_package( SBE ). In that case SBE_STATIC will also be defined
# by this script. Example:
#
# set( SBE_STATIC_LIBRARIES TRUE )
# find_package( SBE )
#
# If SBE is not installed in a standard path, you can use the SBEDIR or
# SBE_ROOT CMake (or environment) variables to tell CMake where to look for
# SBE.
#
#
# OUTPUT
#
# This script defines the following variables:
#   - SBE_LIBRARY_DEBUG:   the path to the debug library
#   - SBE_LIBRARY_RELEASE: the path to the release library
#   - SBE_LIBRARY:         the path to the library to link to
#   - SBE_FOUND:           true if the SBE library is found
#   - SBE_INCLUDE_DIR:     the path where SBE headers are located (the directory containing the SBE/Config.hpp file)
#
#
# EXAMPLE
#
# find_package( SBE REQUIRED )
# include_directories( ${SBE_INCLUDE_DIR} )
# add_executable( myapp ... )
# target_link_libraries( myapp ${SBE_LIBRARY} ... )

set( SBE_FOUND false )

if(DEFINED SBEDIR)
    message("Using SBEDIR: ${SBEDIR}")
endif()

if(DEFINED SBE_ROOT)
    message("Using SBE_ROOT: ${SBE_ROOT}")
endif()

if( SBE_STATIC_LIBRARIES )
	set( SBE_SUFFIX "-s" )
	add_definitions( -DSBE_STATIC )
else()
	set( SBE_SUFFIX "" )
endif()

set( FIND_SBE_PATHS
		${SBEDIR}
		${SBE_ROOT}
		${CMAKE_CURRENT_LIST_DIR}/../
		$ENV{SBEDIR}
		$ENV{SBE_ROOT}
		/usr
		/usr/local)

set( FIND_SBE_PATH_SUFFIXES
		lib
		lib64
		build
		build.win
		cmake-build-debug
		cmake-build-release)

if(CMAKE_SYSTEM_NAME MATCHES Windows)
	#only search for libsbe... as there is a sbe.dll in System32..
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".dll.a" ".a" ".lib")
endif()
		
#always search first for the manual paths, then the default paths
find_path( SBE_INCLUDE_DIR sbe/Engine.hpp PATH_SUFFIXES include PATHS ${FIND_SBE_PATHS} NO_DEFAULT_PATH)
find_path( SBE_INCLUDE_DIR sbe/Engine.hpp PATH_SUFFIXES include )

find_library( SBE_LIBRARY_RELEASE sbe${SBE_SUFFIX} PATH_SUFFIXES ${FIND_SBE_PATH_SUFFIXES} PATHS ${FIND_SBE_PATHS} NO_DEFAULT_PATH )
find_library( SBE_LIBRARY_RELEASE sbe${SBE_SUFFIX} PATH_SUFFIXES ${FIND_SBE_PATH_SUFFIXES} )

find_library( SBE_LIBRARY_DEBUG sbe${SBE_SUFFIX}-d PATH_SUFFIXES ${FIND_SBE_PATH_SUFFIXES} PATHS ${FIND_SBE_PATHS} NO_DEFAULT_PATH )
find_library( SBE_LIBRARY_DEBUG sbe${SBE_SUFFIX}-d PATH_SUFFIXES ${FIND_SBE_PATH_SUFFIXES} )

message("rel: ${SBE_LIBRARY_RELEASE}")
message("dbg: ${SBE_LIBRARY_DEBUG}")

if( SBE_LIBRARY_RELEASE AND SBE_LIBRARY_DEBUG )
	set( SBE_LIBRARY debug ${SBE_LIBRARY_DEBUG} optimized ${SBE_LIBRARY_RELEASE} )
endif()

if( SBE_LIBRARY_RELEASE AND NOT SBE_LIBRARY_DEBUG )
	set( SBE_LIBRARY_DEBUG ${SBE_LIBRARY_RELEASE} )
	set( SBE_LIBRARY ${SBE_LIBRARY_RELEASE} )
endif()

if( SBE_LIBRARY_DEBUG AND NOT SBE_LIBRARY_RELEASE )
	set( SBE_LIBRARY_RELEASE ${SBE_LIBRARY_DEBUG} )
	set( SBE_LIBRARY ${SBE_LIBRARY_DEBUG} )
endif()

set (SBE_ERROR "")

if( NOT SBE_INCLUDE_DIR OR NOT SBE_LIBRARY )

		LIST(APPEND SBE_ERROR "Searching for sbe${SBE_SUFFIX}\n")
		LIST(APPEND SBE_ERROR "Tried suffixes [${CMAKE_FIND_LIBRARY_SUFFIXES} and prefixes [${CMAKE_FIND_LIBRARY_PREFIXES}]\n")
		LIST(APPEND SBE_ERROR "Paths: ${FIND_SBE_PATHS}\n")
		LIST(APPEND SBE_ERROR "Path suffixes: ${FIND_SBE_PATH_SUFFIXES}\n")
		LIST(APPEND SBE_ERROR "SBE not found.\n")

	if( SBE_FIND_REQUIRED )
		message( FATAL_ERROR "${SBE_ERROR}" )
	elseif( NOT SBE_FIND_QUIETLY )	
		message( "${SBE_ERROR}" )
	endif()
else()
	set( SBE_FOUND true )
	if ( NOT SBE_FIND_QUIETLY )
		message( STATUS "SBE found: ${SBE_INCLUDE_DIR}\n with ${SBE_LIBRARY}" )
	endif()
endif()
