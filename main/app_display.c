#include "app.h"
#include "lcd.h"
//#include "lvgl.h"

static void display_init(void)
{
    lcd_init();
}

void app_display_start(void)
{
    display_init();
    //lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
}