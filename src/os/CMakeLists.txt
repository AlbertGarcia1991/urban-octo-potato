idf_component_register(SRCS "tasks.c" "startup.c"
                        PRIV_REQUIRES mincli esp_driver_uart vfs
                        INCLUDE_DIRS "include")
