#ifndef POSTGRES_HANDLER_H
#define POSTGRES_HANDLER_H

#include <libpq-fe.h>

char *executeQueryToJson(const char *query);

char *formatResultAsJson(PGresult *result);

#endif