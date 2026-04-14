set(CMAKE_BUILD_WITH_INSTALL_RPATH false)

add_executable(sbe_tests
    ThreadPoolTest.cpp
)

target_link_libraries(sbe_tests
    PRIVATE
        SBE
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(sbe_tests
    DL_PATHS "${CMAKE_BINARY_DIR}"
)
