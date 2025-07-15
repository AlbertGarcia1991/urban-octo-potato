idf_component_register(
    SRCS "tasks.c" "startup.c"
    PRIV_REQUIRES bt cli uart nvs ble
    INCLUDE_DIRS "include"
)
