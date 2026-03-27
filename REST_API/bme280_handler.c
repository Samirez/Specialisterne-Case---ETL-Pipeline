#include "bme280_handler.h"
#include "headers/postgres_handler.h"
#include "headers/response_builder.h"
#include "headers/utils.h"

HTTP_response get_all_BME280(const char* url)
{
    (void)url;
    const char* query = "SELECT * FROM bme280_data";
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}

HTTP_response get_BME280_by_id(const char* url)
{
    int id;
    sscanf(url, "/bme280/%d", &id);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM bme280_data WHERE id = %d", id);
    char* jsonResult = executeQueryToJson(query);
    if (jsonResult == NULL) {
        HTTP_response response = {NULL, INTERNAL_SERVER_ERROR};
        return response;
    }
    HTTP_response response = {jsonResult, OK};
    return response;
}


HTTP_response bme280_router(const char* url, const char* method, const char* body)
{
    char *user_id = strstr(url, "/bme280/");
  if (user_id != NULL) 
  {
    user_id += strlen("/bme280/");
  }
  
  if (validate_method(method, "GET")) 
  {
      if(user_id == NULL){
        return get_all_BME280(url);
      } else {
        return get_BME280_by_id(user_id);
      }
  }
  HTTP_response response = {NULL, METHOD_NOT_ALLOWED};
  return response;
}


