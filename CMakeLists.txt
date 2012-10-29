project(SchiffbruchEngine)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})

if(CMAKE_SYSTEM_NAME MATCHES Windows)
  set(GENERIC_LIBS pthread jpeg GLEW freetype openal32 sndfile)
  #useless wont be known in a project that uses sbe
  #link_directories( ${CUSTOM_SFML_ROOT}/extlibs/libs-mingw/ )
else()
  set(GENERIC_LIBS pthread jpeg GLEW X11 Xrandr freetype rt openal sndfile)
endif()

option(USE_CUSTOM_BOOST "" OFF)
option(USE_CUSTOM_SFML "" ON)
set(CUSTOM_BOOST_ROOT "" CACHE PATH "custom boost build root dir")
set(CUSTOM_SFML_ROOT "${PROJECT_SOURCE_DIR}/SFGUI/extlibs/SFML" CACHE PATH "custom SFML build root dir")

set(SFGUI_BUILD_EXAMPLES false CACHE BOOL "Do or dont build examples)")
set( BUILD_SHARED_LIBS false CACHE BOOL "Build dynamic library." )
set( SFML_STATIC_LIBRARIES true CACHE BOOL "Was SFML built as a static library?" )

SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

if(USE_CUSTOM_BOOST)
	message("-- Using custom boost...")
	set(BOOST_ROOT "${CUSTOM_BOOST_ROOT}")
endif()

if(USE_CUSTOM_SFML)
	message("-- Using custom SFML...")
	set(SFML_ROOT "${CUSTOM_SFML_ROOT}/build")
	set(SFML_INCLUDE_DIR "${CUSTOM_SFML_ROOT}/include")
endif(USE_CUSTOM_SFML)

find_package(Boost 1.46 COMPONENTS thread system)# regex)
#only audio required, the others are pulled in by sfgui
find_package(SFML 2 COMPONENTS audio)
#find_package(SFML 2 COMPONENTS graphics window system audio)

foreach(LIB ${GENERIC_LIBS})
	find_library(FOUND_${LIB} ${LIB} )
endforeach()

if(NOT Boost_FOUND)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
	find_package(Boost REQUIRED thread system) #regex)
endif()

if(NOT SFML_FOUND)
	message( FATAL_ERROR "Please build SFML in '${CUSTOM_SFML_ROOT}/build' first!")
endif()

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
endif()
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
endif()

#option(SHARED OFF)

add_definitions(-DCOUNT_EVENTS)
if (GCC_VERSION VERSION_GREATER 4.6)
    add_definitions ( -std=c++11 )
else()
    add_definitions ( -std=c++0x )
endif()

link_directories(${SFML_ROOT})
#link_directories("/usr/lib")

add_subdirectory(SFGUI)

file( GLOB_RECURSE sbe_SRCS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
file( GLOB_RECURSE sbe_SRCHDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/*.hpp" )

file( GLOB_RECURSE sbe_HDRS RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include/*.hpp" )

message( "-- Found the following SRCS:\n ${sbe_SRCS}" )
message( "-- Found the following HDRS:\n ${sbe_HDRS}" )
message( "-- Found the following SRCHDRS:\n ${sbe_SRCHDRS}" )

if(DEBUG)
  add_definitions(-fomit-frame-pointer -fexpensive-optimizations -O3 -DBOOST_THREAD_USE_LIB)
endif()

if(SHARED)
    add_library(sbe SHARED ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
else()
    add_library(sbe STATIC ${sbe_HDRS} ${sbe_SRCHDRS} ${sbe_SRCS})
    add_definitions(-DSFML_STATIC -DSFGUI_STATIC)
endif()

set_property(GLOBAL PROPERTY SBE_LINK_LIBS "${Boost_LIBRARIES};${SFML_LIBRARIES};${GENERIC_LIBS}")
set_property(GLOBAL PROPERTY SBE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include;${Boost_INCLUDE_DIR};${SFML_INCLUDE_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

target_link_libraries(sbe sfgui)
target_link_libraries(sbe ${SFML_LIBRARIES})
target_link_libraries(sbe ${Boost_LIBRARIES} ${GENERIC_LIBS})
#message("LINK: ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${GENERIC_LIBS}")
