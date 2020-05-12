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
        ping->set_address((char *) &address);
    }

    void TearDown() override {
        printf("TearDown\n");
        delete ping;
    }

};