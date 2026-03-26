#ifndef USER_HANDLER_H
#define USER_HANDLER_H
#include "utils.h"
typedef struct {
  int id;
  char *name;
  unsigned char *password_hash;
  unsigned int password_iterations;
} USER;

// Public password-hashing parameters for callers that allocate buffers.
#define SALT_LENGTH 32
#define HASH_LENGTH 32

// PBKDF2 cost policy.
// Effective iteration count is read from env var PBKDF2_ITERATIONS when set,
// and clamped to this minimum for security.
#define PBKDF2_MIN_ITERATIONS 600000
#define PBKDF2_ITERATIONS_ENV_VAR "PBKDF2_ITERATIONS"

HTTP_response create_user(const char* url, const char* upload_data);

// Securely hash a password using PBKDF2-HMAC-SHA256
// Parameters:
//   password: The plaintext password to hash
//   salt_out: Output buffer for the generated salt (must be at least 32 bytes)
//   salt_len: Length of salt_out buffer (should be at least SALT_LENGTH = 32 bytes)
//   hashed_password: Output buffer for the derived key/hash (must be at least 32 bytes)
//   hash_len: Length of hashed_password buffer (should be at least HASH_LENGTH = 32 bytes)
//   iterations_out: Optional output pointer for effective PBKDF2 iteration count used
// Returns: 0 on success, negative value on error
int hash_password(const char *password, unsigned char *salt_out, size_t salt_len, unsigned char *hashed_password, size_t hash_len, unsigned int *iterations_out);

#endif
