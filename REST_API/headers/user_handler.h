#ifndef USER_HANDLER_H
#define USER_HANDLER_H
#include "utils.h"

typedef struct {
  int id;
  char *name;
  char *password_hash;
} USER;

HTTP_response create_user(const char* url, const char* upload_data);

// Securely hash a password using PBKDF2-HMAC-SHA256
// Parameters:
//   password: The plaintext password to hash
//   salt_out: Output buffer for the generated salt (must be at least 32 bytes)
//   salt_len: Length of salt_out buffer (should be at least SALT_LENGTH = 32 bytes)
//   hashed_password: Output buffer for the derived key/hash (must be at least 32 bytes)
//   hash_len: Length of hashed_password buffer (should be at least HASH_LENGTH = 32 bytes)
// Returns: 0 on success, negative value on error
int hash_password(const char *password, unsigned char *salt_out, size_t salt_len, unsigned char *hashed_password, size_t hash_len);

#endif
