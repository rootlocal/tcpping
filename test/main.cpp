#include <gtest/gtest.h>

// The main function.
//
// The idea is to use Google Test to run all the tests we have defined (some
// of them are intended to fail), and then compare the test results
// with the "golden" file.
int main(int argc, char **argv) {

    // We just run the tests, knowing some of them are intended to fail.
    // We will use a separate Python script to compare the output of
    // this program with the golden file.

    // It's hard to test InitGoogleTest() directly, as it has many
    // global side effects.  The following line serves as a sanity test
    // for it.
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}