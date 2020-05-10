#include <iostream>
#include <gtest/gtest.h>
#include "tcpping.h"

TEST(TcpPingTest, address) {

    // Arrange
    TcpPing *ping = new TcpPing;
    ping->set_address((char *) "127.0.0.1");

    // Act

    // Assert
    printf("Before the Assert\n");

    ASSERT_STREQ(ping->get_address(), "127.0.0.1");
    EXPECT_THROW(ping->set_address((char *) "badhost"), std::exception);

    printf("After the Assert\n");

    delete ping;
}

TEST(TcpPingTest, port) {

    // Arrange
    TcpPing *ping = new TcpPing;
    ping->set_port(443);

    // Act

    // Assert
    printf("Before the Assert\n");
    ASSERT_EQ(ping->get_port(), 443);
    printf("After the Assert\n");

    delete ping;
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