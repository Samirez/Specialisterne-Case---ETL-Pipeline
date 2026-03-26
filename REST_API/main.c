#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <microhttpd.h>
#include "headers/handler.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
const int PORT = 8080;
MHD_AccessHandlerCallback callback = default_handler;
static volatile sig_atomic_t shutdown_requested = 0;

static void handle_shutdown_signal(int signal_number)
{
    (void)signal_number;
    shutdown_requested = 1;
}


int main()
{
    signal(SIGINT, handle_shutdown_signal);
    signal(SIGTERM, handle_shutdown_signal);
    
    printf("Starting REST API server on port %d...\n", PORT);

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                            &default_handler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start server\n");
        return EXIT_FAILURE;
    }
    // Placeholder for server loop
    while (!shutdown_requested) {
        #ifdef _WIN32
                Sleep(100);
        #else
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = 100 * 1000 * 1000; // 100ms
                nanosleep(&ts, NULL);
        #endif
    }

    printf("Shutdown requested. Exiting cleanly.\n");
    
    MHD_stop_daemon(daemon);
    return EXIT_SUCCESS;
}
