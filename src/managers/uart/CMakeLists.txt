idf_component_register(SRCS "uart.c"
                        PRIV_REQUIRES esp_driver_uart vfs
                        INCLUDE_DIRS "include")
