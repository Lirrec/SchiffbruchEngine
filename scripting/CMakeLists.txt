
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SWIG_FLAGS "")

set_source_files_properties(Geom.i PROPERTIES CPLUSPLUS ON)
#set_source_files_properties(Geom.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(sbe python Geom.i ../include/sbe/geom/Point.hpp)
swig_link_libraries(sbe ${PYTHON_LIBRARIES})

