idf_component_register(SRCS "tasks.c" "startup.c"
                        PRIV_REQUIRES cli uart nvs
                        INCLUDE_DIRS "include")
