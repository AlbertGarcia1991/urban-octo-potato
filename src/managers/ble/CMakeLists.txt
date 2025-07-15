idf_component_register(SRCS "ble_server.c" "ble_client.c"
                        PRIV_REQUIRES bt os
                        INCLUDE_DIRS "include")
