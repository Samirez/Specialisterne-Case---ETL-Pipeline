#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <microhttpd.h>
#include "headers/handler.h"

const int PORT = 8080;

// Forward declaration or ensure default_handler is defined in handler.h
extern int default_handler(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);

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

    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, &default_handler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start HTTP server\n");
        return EXIT_FAILURE;
    }
    // Placeholder for server loop
    while (!shutdown_requested) {
        // Handle incoming requests
        // This is where you would accept connections and process them

    }

    printf("Shutdown requested. Exiting cleanly.\n");
    
    MHD_stop_daemon(daemon);
    return EXIT_SUCCESS;
}
