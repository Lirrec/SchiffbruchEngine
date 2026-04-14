set(CMAKE_BUILD_WITH_INSTALL_RPATH false)

find_package(Catch2 3 REQUIRED)

add_executable(sbe_tests
    TestMain.cpp
    ThreadPoolTest.cpp
    EventSystemTest.cpp
)

target_link_libraries(sbe_tests
    PRIVATE
        SBE
        Catch2::Catch2WithMain
)

# Expose private SBE headers (e.g. event/EventCore.hpp) needed by tests.
target_include_directories(sbe_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"   # config.h lives here
)

include(CTest)
include(Catch)
catch_discover_tests(sbe_tests
    DL_PATHS "${CMAKE_BINARY_DIR}"
)
