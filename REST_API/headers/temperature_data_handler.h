#ifndef TEMPERATURE_DATA_HANDLER_H
#define TEMPERATURE_DATA_HANDLER_H
#include "utils.h"

HTTP_response get_DMI_by_temperature_range(const char* url);

HTTP_response get_DS18B20_by_temperature_range(const char* url);

HTTP_response get_BME280_by_temperature_range(const char* url);

HTTP_response get_SCD41_by_temperature_range(const char* url);

#endif