# we will use the network to fetch Google Test sources
# make it possible to disable unit tests when not on network
option(ENABLE_UNIT_TESTS "Enable unit tests" OFF) # cmake -D ENABLE_UNIT_TESTS=[OFF|ON] ..
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

include(cmake/googletest.cmake)

if (ENABLE_UNIT_TESTS)

    # we use this to get code coverage
    if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    endif ()

    fetch_googletest(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest)

    add_executable(unit_tests
            googletest-tcpping-test.cpp
            )

    target_sources(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpping.cpp)
    target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_link_libraries(unit_tests PRIVATE gtest_main gtest gmock gmock_main)

    add_test(unit ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)
endif ()