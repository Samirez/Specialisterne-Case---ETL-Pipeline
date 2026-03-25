#ifndef USER_HANDLER_H
#define USER_HANDLER_H
#include "utils.h"

typedef struct {
  int id;
  char *name;
  char *password_hash;
} USER;

HTTP_response create_user(const char* url, const char* upload_data);

void hash_password(const char *password, unsigned char *hashed_password);

#endif
