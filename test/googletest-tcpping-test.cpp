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