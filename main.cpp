#include <getopt.h>
#include "tcpping.h"

static volatile int keepRunning = 1;

void intHandler(int signal) {
    printf("\n(caught signal %d)\n", signal);
    keepRunning = 0;
}

void display_usage(char *name) {
    printf("syntax: %s [-?] [-h host | ip] [-p port] [-c count]\n", name);
    printf("\t [--help] | [-?] Displays usage information\n");
    printf("\t [--host <host> | <ip>] | [-h <host> | <ip> Hostname or IP address destination\n");
    printf("\t [--port <port>] | [-p <port>] Port number\n");
    printf("\t [--count <count>] | [-c <count>] Count send packets\n");
}

int main(int argc, char *argv[]) {
    TcpPing *ping;
    int pingLastRunStatus = 0;
    ping = new TcpPing;

    // Ctrl+C handler
    signal(SIGINT, intHandler);

    try {

        if (argc <= 1) {
            display_usage(argv[0]);
            exit(EXIT_FAILURE);
        }

        int c, countPackets = 0, longIndex = 0;

        static const char *optString = "h:p:c:?v";
        static const struct option longOpts[] = {
                {"host",  required_argument, NULL, 'h'},
                {"port",  required_argument, NULL, 'p'},
                {"count", required_argument, NULL, 'c'},
                {"help",  no_argument,       NULL, '?'},
                {NULL,    no_argument,       NULL, 0}
        };

        while ((c = getopt_long(argc, argv, optString, longOpts, &longIndex)) != -1) {
            switch (c) {
                case 'h':
                    ping->set_address(optarg);
                    break;
                case 'p':
                    ping->set_port(atoi(optarg));
                    break;
                case 'c':
                    countPackets = atoi(optarg);
                    break;
                case '?':
                    display_usage(argv[0]);
                    exit(EXIT_SUCCESS);
                default:
                    break;
            }
        }

        while (keepRunning) {
            pingLastRunStatus = ping->run();

            if (countPackets > 0 && ping->get_seq() == countPackets) {
                keepRunning = 0;
            }
        }

    } catch (const std::exception &e) {
        printf("Error %s\n", e.what());
        delete ping;
        exit(EXIT_FAILURE);
    }

    delete ping;
    exit(pingLastRunStatus);
}