#ifndef NVS_H
#define NVS_H

#include "esp_err.h"

esp_err_t nvs_manager_init(void);
esp_err_t nvs_manager_write_int(const char *key, int32_t value);
esp_err_t nvs_manager_read_int(const char *key, int32_t *out_value);
esp_err_t nvs_manager_write_str(const char *key, const char *value);
esp_err_t nvs_manager_read_str(const char *key, char *out_value, size_t max_len);
esp_err_t nvs_manager_erase_key(const char *key);
esp_err_t nvs_manager_erase_all(void);

#endif // NVS_H