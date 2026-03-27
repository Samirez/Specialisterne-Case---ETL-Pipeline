#ifndef PRESSURE_DATA_HANDLER_H
#define PRESSURE_DATA_HANDLER_H

#include "utils.h"

HTTP_response get_DMI_by_pressure_range(const char* url);

HTTP_response get_BME280_by_pressure_range(const char* url);

HTTP_response pressure_router(const char* url, const char* method);

#endif