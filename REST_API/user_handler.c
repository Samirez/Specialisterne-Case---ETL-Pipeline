#include "user_handler.h"
#include <errno.h>
#include <limits.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

static void secure_memzero(void *ptr, size_t len) {
    volatile unsigned char *p = (volatile unsigned char *)ptr;
    while (len--) {
        *p++ = 0;
    }
}

HTTP_response create_user(const char* url, const char* upload_data) {
    (void)url;
    if (upload_data == NULL) {
        HTTP_response response = {simple_message("Invalid request data"), BAD_REQUEST};
        return response;
    }

    // Parse upload_data to extract username and password
    char username[256] = {0};
    char password[256] = {0};
    int parsed = sscanf(upload_data, "username=%255[^&]&password=%255s", username, password);
    if (parsed != 2) {
        HTTP_response response = {simple_message("Missing username or password"), BAD_REQUEST};
        return response;
    }
    
    // Hash the password securely with salt
    unsigned char hashed_password[32];
    unsigned char salt[32];
    int password_iterations = 0;
    int hash_result = hash_password(password, salt, sizeof(salt), hashed_password, sizeof(hashed_password), &password_iterations);
    
    if (hash_result != 0) {
        secure_memzero(password, sizeof(password));
        secure_memzero(salt, sizeof(salt));
        secure_memzero(hashed_password, sizeof(hashed_password));

        HTTP_response response = {simple_message("Error hashing password"), INTERNAL_SERVER_ERROR};
        return response;
    }

    // To do - Store the user in the database with salt, hashed_password, and password_iterations.
    secure_memzero(password, sizeof(password));
    secure_memzero(salt, sizeof(salt));
    secure_memzero(hashed_password, sizeof(hashed_password));
    
    char response_message[512];
    snprintf(response_message, sizeof(response_message), "User created successfully!");    
    HTTP_response response = {simple_message(response_message), OK};
    return response;
}

static int resolve_pbkdf2_iterations(void) {
    const char *env_value = getenv(PBKDF2_ITERATIONS_ENV_VAR);
    long parsed_iterations;
    char *endptr = NULL;

    if (env_value == NULL || env_value[0] == '\0') {
        return PBKDF2_MIN_ITERATIONS;
    }

    errno = 0;
    parsed_iterations = strtol(env_value, &endptr, 10);
    if (errno != 0 || endptr == env_value || *endptr != '\0') {
        return PBKDF2_MIN_ITERATIONS;
    }

    if (parsed_iterations < PBKDF2_MIN_ITERATIONS || parsed_iterations > INT_MAX) {
        return PBKDF2_MIN_ITERATIONS;
    }

    return (int)parsed_iterations;
}

int hash_password(const char *password, unsigned char *salt_out, size_t salt_len,
                  unsigned char *hashed_password, size_t hash_len, int *iterations_out) {
    int pbkdf2_iterations;

    if (!password || !salt_out || !hashed_password) {
        return -1;
    }

    if (salt_len < SALT_LENGTH || hash_len < HASH_LENGTH) {
        return -2;
    }

    if (RAND_bytes(salt_out, SALT_LENGTH) != 1) {
        return -3;
    }

    pbkdf2_iterations = resolve_pbkdf2_iterations();
    if (iterations_out != NULL) {
        *iterations_out = pbkdf2_iterations;
    }

    if (PKCS5_PBKDF2_HMAC(password,
                          (int)strlen(password),
                          salt_out,
                          SALT_LENGTH,
                          pbkdf2_iterations,
                          EVP_sha256(),
                          HASH_LENGTH,
                          hashed_password) != 1) {
        return -4;
    }

    return 0;
}