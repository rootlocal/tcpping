#include <getopt.h>

#if defined(__FreeBSD__) || defined(__linux__)
#include <signal.h>
#endif

#include "tcpping.h"
#include "version.h"

static volatile int keepRunning = 1;

void intHandler(int signal) {
    printf("\n(caught signal %d)\n", signal);
    keepRunning = 0;
}

void display_version() {
    printf("Ver: %d.%d.%d ", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("Rev: %s\n", GIT_HASH);
    printf("BUILD_TYPE: %s\n", BUILD_TYPE);
}

void display_usage(char *name) {
    printf("syntax: %s [-?] [-h host | ip] [-p port] [-c count]\n", name);
    printf("\t [--help] | [-?] Displays usage information\n");
    printf("\t [--host <host> | <ip>] | [-h <host> | <ip> Hostname or IP address destination\n");
    printf("\t [--port <port>] | [-p <port>] Port number\n");
    printf("\t [--count <count>] | [-c <count>] Count send packets\n");
    printf("\t [--version] | [-V] Display version\n");
    display_version();
}

int main(int argc, char *argv[]) {

    TcpPing *ping;
    int pingLastRunStatus = 0;
    unsigned int countPackets = 0;
    int c, longIndex = 0;

    ping = new TcpPing;

    // Ctrl+C handler
    signal(SIGINT, intHandler);

    try {

        if (argc <= 1) {
            display_usage(argv[0]);
            exit(EXIT_FAILURE);
        }


        static const char *optString = "h:p:c:?V";
        static const struct option longOpts[] = {
                {"host",    required_argument, NULL, 'h'},
                {"port",    required_argument, NULL, 'p'},
                {"count",   required_argument, NULL, 'c'},
                {"help",    no_argument,       NULL, '?'},
                {"version", no_argument,       NULL, 'V'},
                {NULL,      no_argument,       NULL, 0}
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
                    countPackets = static_cast<unsigned int> (atoi(optarg));
                    break;
                case '?':
                    display_usage(argv[0]);
                    exit(EXIT_SUCCESS);
                case 'V':
                    display_version();
                    exit(EXIT_SUCCESS);
                default:
                    break;
            }
        }

        printf("Connecting to %s (%s)\n", ping->get_host(), ping->get_address());

        while (keepRunning) {
            pingLastRunStatus = ping->run();

            if (countPackets > 0 && ping->get_seq() == countPackets) {
                keepRunning = 0;
            }
        }

        ping->statistics();

    } catch (const std::exception &e) {
        printf("Error %s\n", e.what());
        delete ping;
        exit(EXIT_FAILURE);
    }

    delete ping;
    exit(pingLastRunStatus);
}