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

if( SBE_STATIC_LIBRARIES )
	set( SBE_SUFFIX "-s" )
	add_definitions( -DSBE_STATIC )
else()
	set( SBE_SUFFIX "" )
endif()

set( FIND_SBE_PATHS
		/usr
		/usr/local
		${SBEDIR}
		${SBE_ROOT}
		$ENV{SBEDIR}
		$ENV{SBE_ROOT})
		
find_path(
	SBE_INCLUDE_DIR
	sbe/Engine.hpp
	PATH_SUFFIXES
		include
	PATHS
		${FIND_SBE_PATHS}
	NO_DEFAULT_PATH
)

find_library(
	SBE_LIBRARY_RELEASE
	sbe${SBE_SUFFIX}
	PATH_SUFFIXES
		lib
		lib64
		build
		build.win
	PATHS
		${FIND_SBE_PATHS}
	NO_DEFAULT_PATH
)

find_library(
	SBE_LIBRARY_DEBUG
	sbe${SBE_SUFFIX}-d
	PATH_SUFFIXES
		lib
		lib64
		build
		build.win
	PATHS
		${FIND_SBE_PATHS}
	NO_DEFAULT_PATH
)

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

if( NOT SBE_INCLUDE_DIR OR NOT SBE_LIBRARY )
	if( SBE_FIND_REQUIRED )
		message( FATAL_ERROR "SBE not found." )
	elseif( NOT SBE_FIND_QUIETLY )
		message( "SBE not found." )
	endif()
else()
	set( SBE_FOUND true )
	if ( NOT SBE_FIND_QUIETLY )
		message( STATUS "SBE found: ${SBE_INCLUDE_DIR} with ${SBE_LIBRARY}" )
	endif()
endif()
