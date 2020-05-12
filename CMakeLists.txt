cmake_minimum_required(VERSION 3.9)

project(tcpping VERSION 1.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

#specify where to place binaries and libraries
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

#include(cmake/debugCompileOptions.cmake)

# Processor Count
include(cmake/processorCount.cmake)
ProcessorCount(N)
if (NOT N EQUAL 0)
    message(STATUS "Processor count: ${N}")
    set(CTEST_BUILD_FLAGS -j${N})
    set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif ()

# Cross compiling
if ("${TARGET_CPU}" STREQUAL "x86")
    include(cmake/x86.cmake)
endif ()

# contains a workaround for macOS
include(cmake/arch.cmake)
macos_workaround()

include(cmake/version.cmake)
generate_version_h()

option(ENABLE_UNIT_TESTS "Enable unit tests" OFF) # cmake -D ENABLE_UNIT_TESTS=[OFF|ON] ..
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")
if (ENABLE_UNIT_TESTS)
    include(test/cmake/codeCoverage.cmake)
    setup_target_for_coverage(coverage)
    # enable testing
    enable_testing()        # <- we added this
    # process test/CMakeLists.txt
    add_subdirectory(test)  # <- we added this
endif ()

# process src/CMakeLists.txt
add_subdirectory(src)

message(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

