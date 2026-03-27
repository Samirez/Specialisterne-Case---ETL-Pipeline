#include "pressure_data_handler.h"
#include "headers/postgres_handler.h"
#include "headers/response_builder.h"
#include "headers/utils.h"

HTTP_response get_DMI_by_pressure_range(const char* url)
{
    float min_pressure, max_pressure;
    sscanf(url, "/dmi/pressure?min=%f&max=%f", &min_pressure, &max_pressure);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM dmi_data WHERE pressure >= %f AND pressure <= %f", min_pressure, max_pressure);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_BME280_by_pressure_range(const char* url)
{
    float min_pressure, max_pressure;
    sscanf(url, "/bme280/pressure?min=%f&max=%f", &min_pressure, &max_pressure);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM bme280_data WHERE pressure >= %f AND pressure <= %f", min_pressure, max_pressure);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response pressure_router(const char* url, const char* method)
{
    if (strncmp(url, "/dmi/pressure", strlen("/dmi/pressure")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_DMI_by_pressure_range(url);
    }

    if (strncmp(url, "/bme280/pressure", strlen("/bme280/pressure")) == 0) {
        if (!validate_method(method, "GET")) {
            HTTP_response method_not_allowed = {simple_message("Method not allowed"), METHOD_NOT_ALLOWED};
            return method_not_allowed;
        }
        return get_BME280_by_pressure_range(url);
    }

    HTTP_response not_found = {simple_message("Not found"), NOT_FOUND};
    return not_found;
}

