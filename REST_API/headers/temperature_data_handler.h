#ifndef TEMPERATURE_DATA_HANDLER_H
#define TEMPERATURE_DATA_HANDLER_H
#include "utils.h"

// Expects "/dmi/temperature?min=<float>&max=<float>"; parsing happens in
// get_temperature_by_range() in REST_API/temperature_data_handler.c.// Expects "/dmi/temperature?min=<float>&max=<float>"; parsing happens in
// get_temperature_by_range() in REST_API/temperature_data_handler.c.
HTTP_response get_DMI_by_temperature_range(const char* url);

// Expects "/ds18b20/temperature?min=<float>&max=<float>"; parsing happens in
// get_temperature_by_range() in REST_API/temperature_data_handler.c.
HTTP_response get_DS18B20_by_temperature_range(const char* url);

// Expects "/bme280/temperature?min=<float>&max=<float>"; parsing happens in
// get_temperature_by_range() in REST_API/temperature_data_handler.c.
HTTP_response get_BME280_by_temperature_range(const char* url);

// Expects "/scd41/temperature?min=<float>&max=<float>"; parsing happens in
// get_temperature_by_range() in REST_API/temperature_data_handler.c.
HTTP_response get_SCD41_by_temperature_range(const char* url);


HTTP_response temperature_router(const char* url, const char* method);
#endif