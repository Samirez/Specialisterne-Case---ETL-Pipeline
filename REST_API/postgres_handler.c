#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "headers/postgres_handler.h"

static int ensure_capacity(char **buffer, size_t *capacity, size_t required_len) {
    if (required_len + 1 <= *capacity) {
        return 0;
    }

    size_t new_capacity = *capacity ? *capacity : 1;
    while (required_len + 1 > new_capacity) {
        if (new_capacity > SIZE_MAX / 2) {
            return -1;  // Overflow protection
        }
        new_capacity *= 2;
    }

    char *new_buffer = (char *)realloc(*buffer, new_capacity);
    if (new_buffer == NULL) {
        return -1;
    }

    *buffer = new_buffer;
    *capacity = new_capacity;
    return 0;
}
static int append_char(char **buffer, size_t *length, size_t *capacity, char c) {
    if (ensure_capacity(buffer, capacity, *length + 1) != 0) {
        return -1;
    }

    (*buffer)[*length] = c;
    (*length)++;
    (*buffer)[*length] = '\0';
    return 0;
}

static int append_str(char **buffer, size_t *length, size_t *capacity, const char *text) {
    size_t text_len = strlen(text);
    if (ensure_capacity(buffer, capacity, *length + text_len) != 0) {
        return -1;
    }

    memcpy(*buffer + *length, text, text_len);
    *length += text_len;
    (*buffer)[*length] = '\0';
    return 0;
}

static int append_json_escaped(char **buffer, size_t *length, size_t *capacity, const char *text) {
    for (size_t i = 0; text[i] != '\0'; ++i) {
        unsigned char ch = (unsigned char)text[i];
        switch (ch) {
            case '"':
                if (append_str(buffer, length, capacity, "\\\"") != 0) return -1;
                break;
            case '\\':
                if (append_str(buffer, length, capacity, "\\\\") != 0) return -1;
                break;
            case '\b':
                if (append_str(buffer, length, capacity, "\\b") != 0) return -1;
                break;
            case '\f':
                if (append_str(buffer, length, capacity, "\\f") != 0) return -1;
                break;
            case '\n':
                if (append_str(buffer, length, capacity, "\\n") != 0) return -1;
                break;
            case '\r':
                if (append_str(buffer, length, capacity, "\\r") != 0) return -1;
                break;
            case '\t':
                if (append_str(buffer, length, capacity, "\\t") != 0) return -1;
                break;
            default:
                if (ch < 0x20) {
                    char escaped[7];
                    snprintf(escaped, sizeof(escaped), "\\u%04x", ch);
                    if (append_str(buffer, length, capacity, escaped) != 0) return -1;
                } else {
                    if (append_char(buffer, length, capacity, (char)ch) != 0) return -1;
                }
                break;
        }
    }

    return 0;
}

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

    const char *keywords[] = {"user", "password", "dbname", "host", "port", NULL};
    const char *values[] = {dbUser, dbPassword, dbName, dbHost, dbPort, NULL};
    PGconn *conn = PQconnectdbParams(keywords, values, 0);

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

    size_t initial_capacity = 128;
    char *json = (char *)malloc(initial_capacity);
    size_t length = 0;
    size_t capacity = initial_capacity;

    if (json == NULL) {
        return NULL;
    }

    json[0] = '\0';

    if (numRows == 0) {
        if (append_str(&json, &length, &capacity, "[]") != 0) {
            free(json);
            return NULL;
        }
        return json;
    }

    if (append_char(&json, &length, &capacity, '[') != 0) {
        free(json);
        return NULL;
    }

    for (int i = 0; i < numRows; ++i) {
        if (i > 0) {
            if (append_char(&json, &length, &capacity, ',') != 0) {
                free(json);
                return NULL;
            }
        }

        if (append_char(&json, &length, &capacity, '{') != 0) {
            free(json);
            return NULL;
        }

        for (int j = 0; j < numFields; ++j) {
            char *fieldName = PQfname(result, j);

            if (j > 0) {
                if (append_char(&json, &length, &capacity, ',') != 0) {
                    free(json);
                    return NULL;
                }
            }

            if (append_char(&json, &length, &capacity, '"') != 0) {
                free(json);
                return NULL;
            }
            if (append_json_escaped(&json, &length, &capacity, fieldName) != 0) {
                free(json);
                return NULL;
            }
            if (append_str(&json, &length, &capacity, "\":") != 0) {
                free(json);
                return NULL;
            }

            if (PQgetisnull(result, i, j)) {
                if (append_str(&json, &length, &capacity, "null") != 0) {
                    free(json);
                    return NULL;
                }
            } else {
                char *fieldValue = PQgetvalue(result, i, j);
                if (append_char(&json, &length, &capacity, '"') != 0) {
                    free(json);
                    return NULL;
                }
                if (append_json_escaped(&json, &length, &capacity, fieldValue) != 0) {
                    free(json);
                    return NULL;
                }
                if (append_char(&json, &length, &capacity, '"') != 0) {
                    free(json);
                    return NULL;
                }
            }
        }

        if (append_char(&json, &length, &capacity, '}') != 0) {
            free(json);
            return NULL;
        }
    }

    if (append_char(&json, &length, &capacity, ']') != 0) {
        free(json);
        return NULL;
    }

    return json;
}