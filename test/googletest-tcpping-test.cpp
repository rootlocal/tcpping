#include <iostream>
#include <gtest/gtest.h>
#include "tcpping.h"
#include "tcpping_fixture.cpp"

TEST_F(TcpPingFixture, getSetAddress) {
    // Arrange
    char address[15] = "127.0.0.1";
    ping->set_address((char *) &address);

    // Act

    // Assert
    printf("Before the Assert\n");
    ASSERT_STREQ(ping->get_address(), address);
    printf("After the Assert\n");
}

TEST_F(TcpPingFixture, ExceptionAddress) {
    // Arrange
    char address[20] = "badhost";
    // Act

    // Assert
    printf("Before the Assert\n");
    EXPECT_THROW(ping->set_address((char *) address), std::exception);
    printf("After the Assert\n");
}

TEST_F(TcpPingFixture, GetSetPort) {
    // Arrange
    int port = 443;
    ping->set_port(port);

    // Act

    // Assert
    printf("Before the Assert\n");
    ASSERT_EQ(ping->get_port(), port);
    printf("After the Assert\n");
}

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