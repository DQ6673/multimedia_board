set(srcs "app_main.c" "app_display.c" )

set(includes ".")

set(requires 
                "driver"
                "esp_lcd"
                "esp_timer"
                "heap"
                
                "lcd"
                "touch"
                "lvgl"
)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )