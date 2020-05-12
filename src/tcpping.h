#pragma once
#ifndef TCPPING_TCPPING_H
#define TCPPING_TCPPING_H

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>

#if defined(__linux__)
#include <cstring>
#include <climits>
#endif

// I use it mostly for remote servers
#define DEFAULT_PORT 22

class TcpPing {

public:
    TcpPing();

    char *get_address();

    void set_address(char *);

    u_short get_port();

    void set_port(u_short);

    int run();

    void statistics();

    unsigned int get_seq();

    char *get_host();

private:
    struct sockaddr_in addr = {};
    int s; // socket
    unsigned long int timeval_subtract(struct timeval *, struct timeval *);

    unsigned int seq = 0;
    unsigned int cnt_successful = 0;

    // aggregate stats
    struct timeval startTime = {}, endTime = {};

    unsigned long int diffMin = ULONG_MAX;
    unsigned long int diffMax = 0;

    unsigned long int diffAvg;
    unsigned long int diffSum = 0;
    unsigned long int diffSum2 = 0;
    double diffMdev;
    char *host;
};

#endif //TCPPING_TCPPING_H