#include <iostream>
#include <gtest/gtest.h>
#include "tcpping.h"

struct TcpPingFixture : public testing::Test {

public:
    TcpPing *ping;

    static void SetUpTestSUite() {
        printf("SetUpTestSUite\n");

    }

    static void TearDownTestSuite() {
        printf("TearDownTestSuite\n");
    }

    void SetUp() override {
        printf("SetUp\n");
        ping = new TcpPing;
        char address[15] = "127.0.0.1";
        int port = 80;

        ping->set_address((char *) &address);
        ping->set_port(port);
    }

    void TearDown() override {
        printf("TearDown\n");
        delete ping;
    }

};