#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "headers/postgres_handler.h"

char *executeQueryToJson(const char *query) {
    PGconn *conn = PQconnectdb(
        "user=" DB_USER
        " password=" DB_PASSWORD
        " dbname=" DB_NAME
        " host=" DB_HOST
        " port=" DB_PORT
    );

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