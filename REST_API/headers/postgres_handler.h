#ifndef POSTGRES_HANDLER_H
#define POSTGRES_HANDLER_H

#include <libpq-fe.h>

#define DB_NAME "*****"
#define DB_USER "*****"
#define DB_PASSWORD "*****"
#define DB_HOST "***.***.***.***"
#define DB_PORT "****"

char *executeQueryToJson(const char *query);

char *formatResultAsJson(PGresult *result);

#endif