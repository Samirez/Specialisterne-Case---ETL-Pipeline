#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "headers/postgres_handler.h"

static const char *getEnvWithFallback(const char *primary, const char *fallback) {
    const char *value = getenv(primary);
    if (value != NULL && strlen(value) > 0) {
        return value;
    }

    if (fallback != NULL) {
        value = getenv(fallback);
        if (value != NULL && strlen(value) > 0) {
            return value;
        }
    }

    return NULL;
}

char *executeQueryToJson(const char *query) {
    const char *dbUser = getEnvWithFallback("DB_USERNAME", "DB_USER");
    const char *dbPassword = getEnvWithFallback("DB_PASSWORD", NULL);
    const char *dbName = getEnvWithFallback("DB_NAME", NULL);
    const char *dbHost = getEnvWithFallback("DB_HOST", NULL);
    const char *dbPort = getEnvWithFallback("DB_PORT", NULL);

    if (dbUser == NULL || dbPassword == NULL || dbName == NULL || dbHost == NULL || dbPort == NULL) {
        fprintf(stderr, "Database connection configuration is incomplete. Missing:");
        if (dbUser == NULL) {
            fprintf(stderr, " DB_USERNAME/DB_USER");
        }
        if (dbPassword == NULL) {
            fprintf(stderr, " DB_PASSWORD");
        }
        if (dbName == NULL) {
            fprintf(stderr, " DB_NAME");
        }
        if (dbHost == NULL) {
            fprintf(stderr, " DB_HOST");
        }
        if (dbPort == NULL) {
            fprintf(stderr, " DB_PORT");
        }
        fprintf(stderr, "\n");
        return NULL;
    }

    char conninfo[1024];
    snprintf(
        conninfo,
        sizeof(conninfo),
        "user=%s password=%s dbname=%s host=%s port=%s",
        dbUser,
        dbPassword,
        dbName,
        dbHost,
        dbPort
    );

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query execution failed: %s", PQerrorMessage(conn));
        PQclear(result);
        PQfinish(conn);
        return NULL;
    }

    char *jsonResult = formatResultAsJson(result);

    PQclear(result);
    PQfinish(conn);

    return jsonResult;
}

char *formatResultAsJson(PGresult *result) {
    int numFields = PQnfields(result);
    int numRows = PQntuples(result);

    int totalSize = numRows * (2 + numFields * 256) + numRows - 1 + 3;

    char *json = (char *)malloc(totalSize);
    json[0] = '\0';

    if (numRows == 0) {
        strcat(json, "[]");
        return json;
    }

    for (int i = 0; i < numRows; ++i) {
        strcat(json, "{");
        for (int j = 0; j < numFields; ++j) {
            if (j > 0) strcat(json, ",");
            char *fieldName = PQfname(result, j);
            char *fieldValue = PQgetvalue(result, i, j);

            strcat(json, "\"");
            strncat(json, fieldName, totalSize - strlen(json) - 5);
            strcat(json, "\":\"");
            strncat(json, fieldValue, totalSize - strlen(json) - 5);
            strcat(json, "\"");
        }
        strcat(json, "}");
        if (i < numRows - 1) strcat(json, ",");
    }

    return json;
}