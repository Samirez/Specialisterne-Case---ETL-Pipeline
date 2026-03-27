#include "temperature_data_handler.h"
#include "headers/postgres_handler.h"
#include "headers/response_builder.h"
#include "headers/utils.h"

static HTTP_response get_temperature_by_range(const char* url, const char* url_format, const char* table_name)
{
    float min_temp, max_temp;
    if (sscanf(url, url_format, &min_temp, &max_temp) != 2) {
        HTTP_response response = {NULL, BAD_REQUEST};
        return response;
    }
    char query[256];
    snprintf(query, sizeof(query), 
             "SELECT * FROM %s WHERE temperature >= %f AND temperature <= %f", 
             table_name, min_temp, max_temp);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_DMI_by_temperature_range(const char* url)
{
    return get_temperature_by_range(url, "/dmi/temperature?min=%f&max=%f", "dmi_data");
}


HTTP_response get_DS18B20_by_temperature_range(const char* url)
{
    return get_temperature_by_range(url, "/ds18b20/temperature?min=%f&max=%f", "ds18b20_data");
}
    

HTTP_response get_BME280_by_temperature_range(const char* url)
{
    return get_temperature_by_range(url, "/bme280/temperature?min=%f&max=%f", "bme280_data");
}
    

HTTP_response get_SCD41_by_temperature_range(const char* url)
{
    return get_temperature_by_range(url, "/scd41/temperature?min=%f&max=%f", "scd41_data");
}


HTTP_response temperature_router(const char* url, const char* method)
{
    if (validate_method(method, "GET")) {
        if (validate_route(url, "/dmi/temperature?min=%f&max=%f")) {
            return get_DMI_by_temperature_range(url);
        } else if (validate_route(url, "/ds18b20/temperature?min=%f&max=%f")) {
            return get_DS18B20_by_temperature_range(url);
        } else if (validate_route(url, "/bme280/temperature?min=%f&max=%f")) {
            return get_BME280_by_temperature_range(url);
        } else if (validate_route(url, "/scd41/temperature?min=%f&max=%f")) {
            return get_SCD41_by_temperature_range(url);
        }
    }
    HTTP_response not_found = {simple_message("Not found"), NOT_FOUND};
    return not_found;
}