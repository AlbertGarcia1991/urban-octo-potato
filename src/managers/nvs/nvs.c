#include "esp_log.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string.h>

#define NVS_NAMESPACE "storage"

static const char *TAG = "NVS";

// Initialize NVS
esp_err_t nvs_manager_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS Flash init failed: %s", esp_err_to_name(err));
    }
    return err;
}

// Write integer value to NVS
esp_err_t nvs_manager_write_int(const char *key, int32_t value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_i32(handle, key, value);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    return err;
}

// Read integer value from NVS
esp_err_t nvs_manager_read_int(const char *key, int32_t *out_value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_i32(handle, key, out_value);
    nvs_close(handle);
    return err;
}

// Write string value to NVS
esp_err_t nvs_manager_write_str(const char *key, const char *value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    return err;
}

// Read string value from NVS
esp_err_t nvs_manager_read_str(const char *key, char *out_value, size_t max_len) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    size_t required_size = 0;
    err = nvs_get_str(handle, key, NULL, &required_size);
    if (err != ESP_OK) {
        nvs_close(handle);
        return err;
    }
    if (required_size > max_len) {
        nvs_close(handle);
        return ESP_ERR_NVS_INVALID_LENGTH;
    }
    err = nvs_get_str(handle, key, out_value, &required_size);
    nvs_close(handle);
    return err;
}

// Erase a key from NVS
esp_err_t nvs_manager_erase_key(const char *key) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_key(handle, key);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    return err;
}

// Erase all keys in the namespace
esp_err_t nvs_manager_erase_all(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_all(handle);
    if (err == ESP_OK) err = nvs_commit(handle);

    nvs_close(handle);
    return err;
}