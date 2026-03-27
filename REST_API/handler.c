#include "headers/response_builder.h"
#include "headers/user_handler.h"
#include <setjmp.h>
#include "headers/handler.h"
#include "humidity_data_handler.h"
#include "temperature_data_handler.h"
#include "pressure_data_handler.h"

_Thread_local jmp_buf exceptionBuffer;

#define TRY if (setjmp(exceptionBuffer) == 0)
#define CATCH else
void log_api(const char *url, const char *method) {
  printf("[%s] %s\n", method, url);
}

enum MHD_Result default_handler(
  void *cls, struct MHD_Connection *connection, 
  const char *url, const char *method, const char *version,
  const char *upload_data, size_t *upload_data_size, void **con_cls) {
  
  char *url_str = (char *)url;
  char *method_str = (char *)method;
  int ret;
  (void)cls; // silence unused parameter warning
  (void)version;
  (void)upload_data_size;
  (void)con_cls;

  struct MHD_Response *response;
  HTTP_response response_api;

  log_api(url_str, method_str);

  TRY {
    if (strcmp(url_str, "/") == 0) {
      response_api = (HTTP_response){
        .body = simple_message("Hello world!"),
        .status = OK
      };
    }
    
    else if (validate_route(url_str, "/users")) {
      response_api = user_router(url_str, method_str, upload_data);
    }
    
    else if (strncmp(url_str, "/dmi/", 5) == 0 ||
         strncmp(url_str, "/bme280/", 8) == 0 ||
         strncmp(url_str, "/ds18b20/", 9) == 0 ||
         strncmp(url_str, "/scd41/", 7) == 0) {
      if (strstr(url_str, "/humidity") != NULL) {
        response_api = humidity_router(url_str, method_str);
      } else if (strstr(url_str, "/temperature") != NULL) {
        response_api = temperature_router(url_str, method_str);
      } else if (strstr(url_str, "/pressure") != NULL) {
        response_api = pressure_router(url_str, method_str);
      } else {
        response_api = (HTTP_response){
          .body = simple_message("Not found"),
          .status = NOT_FOUND
        };
      }
    }
    
    else {
      response_api = (HTTP_response){
        .body = simple_message("Not found"),
        .status = NOT_FOUND
      };
    }
  } CATCH {
    response_api = (HTTP_response){
      .body = simple_message("Internal server error"),
      .status = INTERNAL_SERVER_ERROR
    };

    printf("Internal server error\n");  }

  response = HTTP_build_response_JSON(response_api.body);

  if (!response) {
    free(response_api.body);
    return MHD_NO;
  }

  ret = MHD_queue_response(connection, response_api.status, response);
  MHD_destroy_response(response);
  free(response_api.body);

  return ret;}