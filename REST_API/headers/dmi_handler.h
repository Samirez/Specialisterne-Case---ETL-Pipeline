#ifndef DMI_HANDLER_H
#define DMI_HANDLER_H

#include "utils.h"

HTTP_response get_all_DMI(const char* url);

HTTP_response get_DMI_by_id(const char* url);

HTTP_response dmi_router(const char *url, const char *method, const char *body);

#endif