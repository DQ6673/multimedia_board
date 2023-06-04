set(srcs "esp_lcd_panel_st7796.c" "lcd.c")

set(includes ".")

set(requires 
                "driver"
                "esp_lcd"
)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )

                       