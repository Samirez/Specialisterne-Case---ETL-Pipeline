#include "temperature_data_handler.h"
#include "headers/postgres_handler.h"
#include "headers/response_builder.h"
#include "headers/utils.h"

HTTP_response get_DMI_by_temperature_range(const char* url)
{
    float min_temp, max_temp;
    sscanf(url, "/dmi/temperature?min=%f&max=%f", &min_temp, &max_temp);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM dmi_data WHERE temperature >= %f AND temperature <= %f", min_temp, max_temp);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}


HTTP_response get_DS18B20_by_temperature_range(const char* url)
{
    float min_temp, max_temp;
    sscanf(url, "/ds18b20/temperature?min=%f&max=%f", &min_temp, &max_temp);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM ds18b20_data WHERE temperature >= %f AND temperature <= %f", min_temp, max_temp);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}


HTTP_response get_BME280_by_temperature_range(const char* url)
{
    float min_temp, max_temp;
    sscanf(url, "/bme280/temperature?min=%f&max=%f", &min_temp, &max_temp);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM bme280_data WHERE temperature >= %f AND temperature <= %f", min_temp, max_temp);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}


HTTP_response get_SCD41_by_temperature_range(const char* url)
{
    float min_temp, max_temp;
    sscanf(url, "/scd41/temperature?min=%f&max=%f", &min_temp, &max_temp);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM scd41_data WHERE temperature >= %f AND temperature <= %f", min_temp, max_temp);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}