#include "headers/humidity_data_handler.h"
#include "headers/postgres_handler.h"
#include "headers/response_builder.h"
#include "headers/utils.h"

HTTP_response get_all_BME280(const char* url)
{
    (void)url;
    const char* query = "SELECT * FROM bme280_data";
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}
HTTP_response get_BME280_by_id(const char* url)
{
    int id;
    sscanf(url, "/bme280/%d", &id);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM bme280_data WHERE id = %d", id);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_all_DMI(const char* url)
{
    (void)url;
    const char* query = "SELECT * FROM dmi_data";
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_DMI_by_id(const char* url)
{
    int id;
    sscanf(url, "/dmi/%d", &id);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM dmi_data WHERE id = %d", id);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_all_DS18B20(const char* url)
{
    (void)url;
    const char* query = "SELECT * FROM ds18b20_data";
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_DS18B20_by_id(const char* url)
{
    int id;
    sscanf(url, "/ds18b20/%d", &id);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM ds18b20_data WHERE id = %d", id);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_all_SCD41(const char* url)
{
    (void)url;
    const char* query = "SELECT * FROM scd41_data";
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_SCD41_by_id(const char* url)
{
    int id;
    sscanf(url, "/scd41/%d", &id);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM scd41_data WHERE id = %d", id);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_BME280_by_humidity_range(const char* url)
{
    // Validate both parsed bounds and ordering before building SQL.
    float min_humidity, max_humidity;
    if (sscanf(url, "/bme280/humidity?min=%f&max=%f", &min_humidity, &max_humidity) != 2) {
        HTTP_response response = {simple_message("Invalid humidity query format. Use ?min=<float>&max=<float>"), BAD_REQUEST};
        return response;
    }
    if (min_humidity > max_humidity) {
        HTTP_response response = {simple_message("Invalid humidity range: min must be <= max"), BAD_REQUEST};
        return response;
    }
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM bme280_data WHERE humidity >= %f AND humidity <= %f", min_humidity, max_humidity);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_DMI_by_humidity_range(const char* url)
{
    // Validate both parsed bounds and ordering before building SQL.
    float min_humidity, max_humidity;
    if (sscanf(url, "/dmi/humidity?min=%f&max=%f", &min_humidity, &max_humidity) != 2) {
        HTTP_response response = {simple_message("Invalid humidity query format. Use ?min=<float>&max=<float>"), BAD_REQUEST};
        return response;
    }
    if (min_humidity > max_humidity) {
        HTTP_response response = {simple_message("Invalid humidity range: min must be <= max"), BAD_REQUEST};
        return response;
    }
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM dmi_data WHERE humidity >= %f AND humidity <= %f", min_humidity, max_humidity);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_SCD41_by_humidity_range(const char* url)
{
    // Validate both parsed bounds and ordering before building SQL.
    float min_humidity, max_humidity;
    if (sscanf(url, "/scd41/humidity?min=%f&max=%f", &min_humidity, &max_humidity) != 2) {
        HTTP_response response = {simple_message("Invalid humidity query format. Use ?min=<float>&max=<float>"), BAD_REQUEST};
        return response;
    }
    if (min_humidity > max_humidity) {
        HTTP_response response = {simple_message("Invalid humidity range: min must be <= max"), BAD_REQUEST};
        return response;
    }
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM scd41_data WHERE humidity >= %f AND humidity <= %f", min_humidity, max_humidity);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response humidity_router(const char* url, const char* method)
{
    if (strncmp(url, "/bme280/", strlen("/bme280/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_all_BME280(url);
    }

    if (strncmp(url, "/bme280/", strlen("/bme280/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_BME280_by_id(url);
    }

    if (strncmp(url, "/dmi/", strlen("/dmi/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_all_DMI(url);
    }

    if (strncmp(url, "/dmi/", strlen("/dmi/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_DMI_by_id(url);
    }

     if (strncmp(url, "/ds18b20/", strlen("/ds18b20/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_all_DS18B20(url);
    }

    if (strncmp(url, "/ds18b20/", strlen("/ds18b20/")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_DS18B20_by_id(url);
    }

    if (strncmp(url, "/bme280/humidity", strlen("/bme280/humidity")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_BME280_by_humidity_range(url);
    }

    if (strncmp(url, "/dmi/humidity", strlen("/dmi/humidity")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_DMI_by_humidity_range(url);
    }

    if (strncmp(url, "/scd41/humidity", strlen("/scd41/humidity")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_SCD41_by_humidity_range(url);
    }

    HTTP_response not_found = {simple_message("Not found"), NOT_FOUND};
    return not_found;
}
