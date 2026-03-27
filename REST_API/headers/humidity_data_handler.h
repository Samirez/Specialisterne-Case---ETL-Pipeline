#ifndef HUMIDITY_DATA_HANDLER_H
#define HUMIDITY_DATA_HANDLER_H

#include "utils.h"



HTTP_response get_all_DS18B20(const char* url);

HTTP_response get_DS18B20_by_id(const char* url);

HTTP_response get_all_SCD41(const char* url);

HTTP_response get_SCD41_by_id(const char* url);

HTTP_response get_BME280_by_humidity_range(const char* url);

HTTP_response get_DMI_by_humidity_range(const char* url);

HTTP_response get_SCD41_by_humidity_range(const char* url);

HTTP_response humidity_router(const char* url, const char* method);

#endif