#ifndef SCD41_HANDLER_H
#define SCD41_HANDLER_H
#include "utils.h"

HTTP_response get_all_SCD41(const char* url);

HTTP_response get_SCD41_by_id(const char* url);

HTTP_response scd41_router(const char *url, const char *method, const char *body);

#endif