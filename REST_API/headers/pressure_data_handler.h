#ifndef TEMPERATURE_DATA_HANDLER_H
#define TEMPERATURE_DATA_HANDLER_H

#include "utils.h"

HTTP_response get_DMI_by_pressure_range(const char* url);

HTTP_response get_BME280_by_pressure_range(const char* url);

#endif