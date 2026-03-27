#ifndef DS18B20_HANDLER_H
#define DS18B20_HANDLER_H

#include "utils.h"

HTTP_response get_all_DS18B20(const char* url);

HTTP_response get_DS18B20_by_id(const char* url);

HTTP_response ds180b20_router(const char *url, const char *method, const char *body);

#endif