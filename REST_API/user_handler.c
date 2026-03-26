#include "user_handler.h"
#include <errno.h>
#include <limits.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

static int hex_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

static int url_decode_into(const char *src, size_t src_len, char *dst, size_t dst_size) {
    size_t i = 0;
    size_t j = 0;

    if (dst_size == 0) {
        return -1;
    }

    while (i < src_len) {
        unsigned char out_char;

        if (src[i] == '+') {
            out_char = ' ';
            i++;
        } else if (src[i] == '%') {
            int hi;
            int lo;
            if (i + 2 >= src_len) {
                return -1;
            }
            hi = hex_value(src[i + 1]);
            lo = hex_value(src[i + 2]);
            if (hi < 0 || lo < 0) {
                return -1;
            }
            out_char = (unsigned char)((hi << 4) | lo);
            i += 3;
        } else {
            out_char = (unsigned char)src[i];
            i++;
        }

        if (j + 1 >= dst_size) {
            return -1;
        }
        dst[j++] = (char)out_char;
    }

    dst[j] = '\0';
    return 0;
}

static int parse_form_credentials(const char *upload_data, char *username, size_t username_size, char *password, size_t password_size) {
    const char *pair_start = upload_data;
    int username_found = 0;
    int password_found = 0;

    while (pair_start != NULL && *pair_start != '\0') {
        const char *pair_end = strchr(pair_start, '&');
        size_t pair_len = pair_end != NULL ? (size_t)(pair_end - pair_start) : strlen(pair_start);
        const char *eq = memchr(pair_start, '=', pair_len);

        if (eq != NULL) {
            size_t key_len = (size_t)(eq - pair_start);
            const char *value_start = eq + 1;
            size_t value_len = pair_len - key_len - 1;

            if (key_len == 8 && strncmp(pair_start, "username", key_len) == 0) {
                if (url_decode_into(value_start, value_len, username, username_size) != 0) {
                    return -1;
                }
                username_found = 1;
            } else if (key_len == 8 && strncmp(pair_start, "password", key_len) == 0) {
                if (url_decode_into(value_start, value_len, password, password_size) != 0) {
                    return -1;
                }
                password_found = 1;
            }
        }

        if (pair_end == NULL) {
            break;
        }
        pair_start = pair_end + 1;
    }

    return (username_found && password_found) ? 0 : -1;
}

HTTP_response create_user(const char* url, const char* upload_data) {
    (void)url;
    if (upload_data == NULL) {
        HTTP_response response = {simple_message("Invalid request data"), BAD_REQUEST};
        return response;
    }

    // Parse form-urlencoded payload and decode percent-encoded credentials.
    char username[256] = {0};
    char password[256] = {0};
    if (parse_form_credentials(upload_data, username, sizeof(username), password, sizeof(password)) != 0) {
        HTTP_response response = {simple_message("Missing username or password"), BAD_REQUEST};
        return response;
    }
    
    // Hash the password securely with salt
    unsigned char hashed_password[32];
    unsigned char salt[32];
    unsigned int password_iterations = 0;
    int hash_result = hash_password(password, salt, sizeof(salt), hashed_password, sizeof(hashed_password), &password_iterations);
    
    if (hash_result != 0) {
        OPENSSL_cleanse(password, sizeof(password));
        OPENSSL_cleanse(salt, sizeof(salt));
        OPENSSL_cleanse(hashed_password, sizeof(hashed_password));

        HTTP_response response = {simple_message("Error hashing password"), INTERNAL_SERVER_ERROR};
        return response;
    }

    // To do - Store the user in the database with salt, hashed_password, and password_iterations.
    // Example: store_result = store_user(username, salt, hashed_password, password_iterations);
    int store_result = 0;

    // Always clear plaintext password as soon as hashing is complete.
    OPENSSL_cleanse(password, sizeof(password));

    if (store_result != 0) {
        OPENSSL_cleanse(salt, sizeof(salt));
        OPENSSL_cleanse(hashed_password, sizeof(hashed_password));

        HTTP_response response = {simple_message("Error storing user"), INTERNAL_SERVER_ERROR};
        return response;
    }

    // Clear derived secrets after persistence completes.
    OPENSSL_cleanse(salt, sizeof(salt));
    OPENSSL_cleanse(hashed_password, sizeof(hashed_password));
    
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
                  unsigned char *hashed_password, size_t hash_len, unsigned int *iterations_out) {
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
        *iterations_out = (unsigned int)pbkdf2_iterations;
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