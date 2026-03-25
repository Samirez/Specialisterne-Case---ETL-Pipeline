#include "headers/utils.h"

char *simple_message(const char *message_str) {
    char *response = (char *)malloc(strlen(message_str) + 1);
    if (response != NULL) {
        strcpy(response, message_str);
    }
    return response;
}

HTTP_response validate_result(char *result) {
    HTTP_response response;
    response.body = result;
    response.status = OK;
    return response;
}

bool validate_method(const char *method, char *valid_method) {
    return strcmp(method, valid_method) == 0;
}

bool validate_route(const char *url, char *route) {
    return strcmp(url, route) == 0;
}
