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
find_program(GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

if (NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif () # NOT GCOV_PATH

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")


MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_COVERAGE
        CMAKE_C_FLAGS_COVERAGE
        CMAKE_EXE_LINKER_FLAGS_COVERAGE
        CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

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


    # Setup target
    add_custom_target(${_targetname}
            # Capturing lcov counters and generating report
            COMMAND ${CMAKE_COMMAND} -E remove coverage.info coverage.info.cleaned
            COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info -rc lcov_branch_coverage=1
            COMMAND ${LCOV_PATH} --remove coverage.info 'tests/*' '/usr/*' '*googletest*' --output-file coverage.info.cleaned
            COMMAND ${GENHTML_PATH} -o coverage coverage.info.cleaned
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
            )

    # Show info where to find the report
    add_custom_command(TARGET ${_targetname} POST_BUILD
            COMMAND ;
            COMMENT "Open ./coverage/index.html in your browser to view the coverage report."
            )

endfunction()