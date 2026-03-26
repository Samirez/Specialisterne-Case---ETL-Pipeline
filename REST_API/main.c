#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "headers/postgres_handler.h"
const int PORT = 8080;


int main()
{
    printf("Starting REST API server on port %d...\n", PORT);
   
    const char *query = "SELECT * FROM DMI"; // Replace with your actual query
    char *jsonResult = executeQueryToJson(query);
    if (jsonResult != NULL) {
        printf("Query result: %s\n", jsonResult);
        free(jsonResult);
    } else {
        fprintf(stderr, "Failed to execute query.\n");
    }

    // Placeholder for server loop
    while (1) {
        // Handle incoming requests
        // This is where you would accept connections and process them

    }

    return 0;
}
