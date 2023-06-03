set(srcs "app_main.c")

set(includes ".")

set(requires 
                "driver"
                "esp_lcd"
                "lcd"
                "touch"
                "lvgl"
)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )