#include <iostream>
#include <gtest/gtest.h>
#include "tcpping.h"
#include "tcpping_fixture.cpp"

TEST_F(TcpPingFixture, getSetAddress) {
    char address[15] = "127.0.0.1";
    ping->set_address((char *) &address);
    ASSERT_STREQ(ping->get_address(), address);
}

TEST_F(TcpPingFixture, ExceptionAddress) {
    char address[20] = "badhost";
    EXPECT_THROW(ping->set_address((char *) &address), std::exception);
}

TEST_F(TcpPingFixture, GetSetPort) {
    int port = 443;

    ASSERT_EQ(ping->get_port(), DEFAULT_PORT);

    ping->set_port(port);
    ASSERT_EQ(ping->get_port(), port);
}

TEST_F(TcpPingFixture, Gethost) {
    char host[15] = "localhost";
    ping->set_address((char *) &host);
    ASSERT_STREQ(ping->get_host(), host);
}

TEST_F(TcpPingFixture, Run) {
    ping->set_address((char *) &"github.com");
    ping->set_port(443);
    int res = ping->run();
    printf("seq=%d", ping->get_seq());
    ASSERT_EQ(res, 0);
}