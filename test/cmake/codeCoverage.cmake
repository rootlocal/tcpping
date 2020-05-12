#
# USAGE:
# Add the following line to your CMakeLists.txt:
#
#    Example:
#    include(codeCoverage)
#	 setup_target_for_coverage(
#				coverage  # Name for custom target.
#				)
#
# Build a Debug build:
#	 cmake -DCMAKE_BUILD_TYPE=Debug ..
#	 make
#	 make coverage

# Check prereqs
find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

if (NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif () # NOT GCOV_PATH

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
#add_compile_options(-Werror)


if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage"))
    message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
endif () # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"

# Param _targetname     The name of new the custom make target
function(setup_target_for_coverage _targetname)

    if (NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif () # NOT LCOV_PATH

    if (NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif () # NOT GENHTML_PATH

    set(exclude_coverage 'test/*' '/usr/*' '*/googletest/*')

    # Setup target
    add_custom_target(${_targetname}
            # Capturing lcov counters and generating report
            COMMAND ${LCOV_PATH} --directory ../ --capture --output-file coverage.info --rc lcov_branch_coverage=1
            COMMAND ${LCOV_PATH} --remove coverage.info ${exclude_coverage} --output-file coverage.info --rc lcov_branch_coverage=1
            COMMAND ${GENHTML_PATH} -o html coverage.info -t ${PROJECT_NAME} --rc lcov_branch_coverage=1
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
            COMMENT "Processing code coverage counters and generating report.\n"
            )

    add_custom_target(make_directory-coverage
            COMMAND ${CMAKE_COMMAND} -E make_directory coverage
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "create coverage directory\n"
            )

    add_custom_target(clean-coverage
            COMMAND ${CMAKE_COMMAND} -E rm -fR *.gcda
            COMMAND ${CMAKE_COMMAND} -E rm -fR coverage
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "coverage clean\n"
            )

    add_dependencies(${_targetname} clean-coverage make_directory-coverage)

    # Show info where to find the report
    add_custom_command(TARGET ${_targetname} POST_BUILD
            COMMAND ;
            COMMENT "Open ${CMAKE_BINARY_DIR}/coverage/html/index.html in your browser to view the coverage report.\n"
            )
endfunction()