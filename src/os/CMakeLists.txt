idf_component_register(SRCS "tasks.c" "startup.c"
                        PRIV_REQUIRES mincli uart
                        INCLUDE_DIRS "include")
