#include "user_handler.h"

HTTP_response create_user(const char* url, const char* upload_data) {
    // Parse upload_data to extract username and password
    char username[256], password[256];
    sscanf(upload_data, "username=%255[^&]&password=%255s", username, password);

    // Hash the password
    unsigned char hashed_password[32];
    hash_password(password, hashed_password);

    // Here you would insert the new user into the database using the hashed password
    // For demonstration, we will just return a success message

    char response_message[512];
    snprintf(response_message, sizeof(response_message), "User '%s' created successfully!", username);
    
    HTTP_response response = {simple_message(response_message), OK};
    return response;
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