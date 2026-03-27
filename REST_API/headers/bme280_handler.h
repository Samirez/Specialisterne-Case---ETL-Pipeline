#ifndef BME280_HANDLER_H
#define BME280_HANDLER_H

#include "utils.h"

HTTP_response get_all_BME280(const char* url);

HTTP_response get_BME280_by_id(const char* url);

HTTP_response bme280_router(const char *url, const char *method, const char *body);

#endif