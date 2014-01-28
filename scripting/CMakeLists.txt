
if ( USE_RUBY OR USE_PYTHON )

	FIND_PACKAGE(SWIG REQUIRED)
	include(${SWIG_USE_FILE})

	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	set(SBE_SCRIPT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}")
	set(CMAKE_SWIG_FLAGS "-c++;-I${PROJECT_SOURCE_DIR}/include")

	set_source_files_properties(sbe.i PROPERTIES CPLUSPLUS ON)
endif()

if ( USE_RUBY )
	FIND_PACKAGE(Ruby REQUIRED)

	set(SBE_SCRIPT_INCLUDES "${SBE_SCRIPT_INCLUDES};${RUBY_INCLUDE_DIR};${RUBY_CONFIG_INCLUDE_DIR}")
	include_directories( ${SBE_SCRIPT_INCLUDES} )

	message(STATUS "Compiling Ruby scripting bindings")
	swig_add_module(SBE ruby
		sbe.i 
		../include/sbe/geom/Point.hpp
		../include/sbe/event/Event.hpp
		../src/event/EventQueue.hpp  
		../include/sbe/event/EventUser.hpp
		../include/sbe/Module.hpp)
	swig_link_libraries(SBE sbe ${RUBY_LIBRARY})
	set(SBE_SCRIPT_LIBS "${RUBY_LIBRARY}")

	set(SBE_SCRIPT_SOURCES "src/scripting/RubyEngine.cpp")
	#set_source_files_properties( "../src/scripting/RubyEngine.cpp" "../include/scripting/RubyEngine.hpp" INCLUDE_DIRECTORIES "${RUBY_INCLUDE_DIR}")
else()
	list(REMOVE_ITEM sbe_SRCS "src/scripting/RubyEngine.cpp")
	list(REMOVE_ITEM sbe_HDRS "include/scripting/RubyEngine.hpp")
endif()
	
	
if( USE_PYTHON )
	FIND_PACKAGE(PythonLibs REQUIRED)
	
	set(SBE_SCRIPT_INCLUDES "${SBE_SCRIPT_INCLUDES};${PYTHON_INCLUDE_PATH}")
	include_directories( ${SBE_SCRIPT_INCLUDES} )
	
	message(STATUS "Compiling Python scripting bindings")
	swig_add_module(SBE python
		sbe.i 
		../include/sbe/geom/Point.hpp
		../include/sbe/event/Event.hpp
		../src/event/EventQueue.hpp  
		../include/sbe/event/EventUser.hpp
		../include/sbe/Module.hpp)
	swig_link_libraries(SBE sbe ${PYTHON_LIBRARIES})
	set(SBE_SCRIPT_LIBS "${SBE_SCRIPT_LIBS};${PYTHON_LIBRARIES}")

	set(SBE_SCRIPT_SOURCES "${SBE_SCRIPT_SOURCES};src/scripting/PythonEngine.cpp")
	#set_property( SOURCE "../src/scripting/PythonEngine.cpp" "../include/scripting/PythonEngine.hpp" APPEND PROPERTY INCLUDE_DIRECTORIES "${PYTHON_INCLUDE_PATH}")
else()
	list(REMOVE_ITEM sbe_SRCS "src/scripting/PythonEngine.cpp")
	list(REMOVE_ITEM sbe_HDRS "include/scripting/PythonEngine.hpp")
endif()

message(STATUS "${SBE_SCRIPT_INCLUDES}:${PYTHON_INCLUDE_PATH}" )

set( SBE_SCRIPT_INCLUDES ${SBE_SCRIPT_INCLUDES} PARENT_SCOPE)
set( SBE_SCRIPT_SOURCES ${SBE_SCRIPT_SOURCES} PARENT_SCOPE)
set( SBE_SCRIPT_LIBS ${SBE_SCRIPT_LIBS} PARENT_SCOPE)
set( sbe_SRCS ${sbe_SRCS} PARENT_SCOPE)
set( sbe_HDRS ${sbe_HDRS} PARENT_SCOPE)

