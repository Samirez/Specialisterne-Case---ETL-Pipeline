#include "headers/utils.h"

char *simple_message(const char *message_str) {
    if (message_str == NULL) {
        return NULL;
    }
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
    if (method == NULL || valid_method == NULL) 
    {
        return false;
    }

    return strcmp(method, valid_method) == 0;
}

bool validate_route(const char *url, char *route) {
    if (url == NULL || route == NULL) {
        return false;
    }
    return strcmp(url, route) == 0;
}

void hash_password(const char *password, unsigned char *hashed_password) {
    if (password == NULL || hashed_password == NULL) {
        return;
    }
    SHA256((unsigned char *)password, strlen(password), hashed_password);
    unsigned char salt[32];
    RAND_bytes(salt, sizeof(salt));
    unsigned char salted_password[256+32];
    memcpy(salted_password, password, strlen(password));
    memcpy(salted_password + strlen(password), salt, sizeof(salt));
}