cmake_minimum_required(VERSION 3.9)

project(tcpping VERSION 1.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#specify where to place binaries and libraries
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# contains a workaround for macOS
include(cmake/arch.cmake)
macos_workaround()

include(cmake/version.cmake)
generate_version_git_h()

# process src/CMakeLists.txt
add_subdirectory(src)

# enable testing
enable_testing()        # <- we added this
# process test/CMakeLists.txt
add_subdirectory(test)  # <- we added this