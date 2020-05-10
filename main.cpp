#include "tcpping.h"

static volatile int keepRunning = 1;

void intHandler(int signal) {
    printf("\n(caught signal %d)\n", signal);
    keepRunning = 0;
}

void display_usage(char *name) {
    printf("syntax: %s [-?] [-h host | ip] [-p port] [-c count]\n", name);
    printf("\t [-?] \t\t\t# Displays usage information\n");
    printf("\t [-h host | ip] \t# Hostname or IP address destination\n");
    printf("\t [-p port] \t\t# Port number\n");
    printf("\t [-c count] \t\t# Count send packets\n");
}

int main(int argc, char *argv[]) {
    TcpPing *ping;
    ping = new TcpPing;

    // Ctrl+C handler
    signal(SIGINT, intHandler);

    try {

        if (argc <= 1) {
            display_usage(argv[0]);
        }

        int c, countPackets = 0;

        while ((c = getopt(argc, argv, "h:p:c:?")) != -1) {
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
                    abort();
            }
        }

        while (keepRunning) {
            ping->run();

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
    exit(EXIT_SUCCESS);
}