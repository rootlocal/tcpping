#include <cmath>
#include "tcpping.h"

unsigned long int TcpPing::timeval_subtract(struct timeval *t2, struct timeval *t1) {
    return (unsigned long int) (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
}

TcpPing::TcpPing() {
    gettimeofday(&startTime, NULL);
}

TcpPing::~TcpPing() {
    gettimeofday(&endTime, NULL);

    // note the ending time and calculate the duration of TCP ping
    long int diff = timeval_subtract(&endTime, &startTime);

    // summary
    printf("\n--- %s:%u tcpping statistics ---\n", get_address(), get_port());
    printf("%d packets transmitted, %d received, %d%% packet loss, time %0.3lf ms\n", seq, cnt_successful,
           100 - (100 * cnt_successful) / seq, diff / 1000.);

    if (cnt_successful > 0) {
        diffAvg = diffSum / cnt_successful;
        diffMdev = sqrt(diffSum2 / cnt_successful - diffAvg * diffAvg);

        printf("rtt min/avg/max/mdev = %0.3lf/%0.3lf/%0.3lf/%0.3lf ms\n", diffMin / 1000., diffAvg / 1000.,
               diffMax / 1000., diffMdev / 1000.);
    }
}

char *TcpPing::get_address() {

    return inet_ntoa(addr.sin_addr);
}

void TcpPing::set_address(char *address) {

    if (!inet_aton(address, &addr.sin_addr)) {

        char error[20];
        hostent *hostent = gethostbyname(address);

        if (hostent == NULL) {
            sprintf(error, "%d Unknown host %s\n", h_errno, address);
            throw std::runtime_error(error);
        }

        memcpy(&(addr.sin_addr.s_addr), hostent->h_addr, (size_t) hostent->h_length);
    }
}

u_short TcpPing::get_port() {

    if (addr.sin_port == 0) {
        set_port(DEFAULT_PORT);
    }

    return ntohs(addr.sin_port);
}

void TcpPing::set_port(int port) {
    addr.sin_port = htons(port);
}

unsigned int TcpPing::get_seq() {
    return seq;
}

int TcpPing::run() {
    // note the starting time
    struct timeval tvBegin = {}, tvEnd = {};

    gettimeofday(&tvBegin, NULL);
    s = socket(PF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        fprintf(stderr, "Failed to create INET socket, error %d\n", errno);
        return 1;
    }

    addr.sin_family = AF_INET;
    seq++;

    // adjusting connection timeout = 1 second
    struct timeval timeout = {};
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, "Failed setsockopt SO_RCVTIMEO, error %d\n", errno);
    }

    if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, "Failed setsockopt SO_SNDTIMEO, error %d\n", errno);
    }

    // try to make connection
    if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) == -1) {

        switch (errno) {

            case EMFILE:
                fprintf(stderr, "Error (%d): too many files opened", errno);
                break;

            case ECONNREFUSED:
                fprintf(stderr, "almost Connection refused (error %d) %s:%d, seq=%d\n", errno, get_address(),
                        get_port(), seq);
                break;

            case EHOSTUNREACH:
                fprintf(stderr, "Host unreachable (error %d) while connecting %s:%d, seq=%d\n", errno, get_address(),
                        get_port(), seq);
                break;

            case EINPROGRESS:
                fprintf(stderr, "Timeout (error %d) while connecting %s:%d, seq=%d\n", errno, get_address(), get_port(),
                        seq);
                break;

            default:
                fprintf(stderr, "Error (%d) while connecting %s:%d, seq=%d\n", errno, get_address(), get_port(), seq);
        }

        close(s);
        // sleeping 1 sec until the next ping
        sleep(1);
        return EXIT_FAILURE;
    }

    // OK, closing the connection
    close(s);

    // note the ending time and calculate the duration of TCP ping
    gettimeofday(&tvEnd, NULL);
    cnt_successful++;

    unsigned long int diff = timeval_subtract(&tvEnd, &tvBegin);
    unsigned long int secs = diff / 1000000;

    printf("OK Connected to %s:%d, seq=%d, time=%0.3lf ms\n", get_address(), get_port(), seq, diff / 1000.);

    // changing aggregate stats
    if (diff < diffMin) {
        diffMin = diff;
    }

    if (diff > diffMax) {
        diffMax = diff;
    }

    diffSum += diff;
    diffSum2 += diff * diff;

    // sleeping until the beginning of the next second
    struct timespec ts = {};
    ts.tv_sec = 0;
    ts.tv_nsec = 1000 * (1000000 * (1 + secs) - diff);
    nanosleep(&ts, &ts);

    return EXIT_SUCCESS;
}