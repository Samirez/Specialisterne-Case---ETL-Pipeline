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
