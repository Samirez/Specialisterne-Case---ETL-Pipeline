#include "user_handler.h"

HTTP_response create_user(const char* url, const char* upload_data) {
    if (upload_data == NULL) {
        HTTP_response response = {simple_message("Invalid request data"), BAD_REQUEST};
        return response;
    }

    // Parse upload_data to extract username and password
    char username[256], password[256];
    int parsed = sscanf(upload_data, "username=%255[^&]&password=%255s", username, password);
    if (parsed != 2) {
        HTTP_response response = {simple_message("Missing username or password"), BAD_REQUEST};
        return response;
    }
    
    // Hash the password securely with salt
    unsigned char hashed_password[32];
    unsigned char salt[32];
    int hash_result = hash_password(password, salt, sizeof(salt), hashed_password, sizeof(hashed_password));
    
    if (hash_result != 0) {
        HTTP_response response = {simple_message("Error hashing password"), INTERNAL_SERVER_ERROR};
        return response;
    }

    // To do - Store the user in the database with both salt and hashed_password (not implemented here)
    
    char response_message[512];
    snprintf(response_message, sizeof(response_message), "User created successfully!");    
    HTTP_response response = {simple_message(response_message), OK};
    return response;
}

#define PBKDF2_ITERATIONS 2024
#define SALT_LENGTH 32
#define HASH_LENGTH 32

int hash_password(const char *password, unsigned char *salt_out, size_t salt_len,
                  unsigned char *hashed_password, size_t hash_len) {
    if (!password || !salt_out || !hashed_password) {
        return -1;
    }

    if (salt_len < SALT_LENGTH || hash_len < HASH_LENGTH) {
        return -2;
    }

    if (RAND_bytes(salt_out, SALT_LENGTH) != 1) {
        return -3;
    }

    if (PKCS5_PBKDF2_HMAC(password,
                          (int)strlen(password),
                          salt_out,
                          SALT_LENGTH,
                          PBKDF2_ITERATIONS,
                          EVP_sha256(),
                          HASH_LENGTH,
                          hashed_password) != 1) {
        return -4;
    }

    return 0;
}