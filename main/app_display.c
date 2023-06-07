#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

#include "iic.h"

#include "esp_heap_caps.h"
#include "freertos/queue.h"
#include "gt911.h"
#include "app.h"
#include "lcd.h"
#include "lvgl.h"
#include "lv_demo_benchmark.h"
#include "demos/lv_demos.h"
#include "examples/lv_examples.h"

const static char *TAG = "app_main";

extern esp_lcd_panel_handle_t lcd_panel_handle;
extern QueueHandle_t lcd_flush_done_queue;
extern gt911_handle_t touch_handle;
extern QueueHandle_t touch_signal_queue;

#define DISP_HOR_RES LCD_RES_H
#define DISP_VER_RES LCD_RES_V
#define DISP_DRAW_BUF_SIZE (DISP_HOR_RES * DISP_VER_RES / 5)

#define LV_TICK_PERIOD_MS 1

/**********************************************************************************************/
static void hal_driver_init(void);
static void display_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void lv_tick_task(void *arg);
/**********************************************************************************************/

/**********************************************************************************************/

/**********************************************************************************************/

void app_display_start(void)
{
    {
        lv_init();

        hal_driver_init();

        lv_color_t *buf1 = heap_caps_malloc(DISP_DRAW_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
        assert(buf1 != NULL);
        lv_color_t *buf2 = heap_caps_malloc(DISP_DRAW_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
        assert(buf2 != NULL);
        static lv_disp_draw_buf_t disp_draw_buf;
        lv_disp_draw_buf_init(&disp_draw_buf, buf1, buf2, DISP_DRAW_BUF_SIZE);

        static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
        lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
        disp_drv.flush_cb = display_flush_cb; /*Set your driver function*/
        disp_drv.draw_buf = &disp_draw_buf;   /*Assign the buffer to the display*/
        disp_drv.hor_res = DISP_HOR_RES;      /*Set the horizontal resolution of the display*/
        disp_drv.ver_res = DISP_VER_RES;      /*Set the vertical resolution of the display*/
        lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

        lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.read_cb = touchpad_read_cb;
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        lv_indev_drv_register(&indev_drv);

        const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            .name = "lvgl tick timer"};
        esp_timer_handle_t periodic_timer;
        ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
    }
    

    lv_example_event_4();

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(5));
        lv_task_handler();
    }
    // lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
}

static void hal_driver_init(void)
{
    lcd_init();
    touch_init();
}

static void display_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    static uint8_t flush_ready_flag;

    // while (gpio_get_level(LCD_PIN_TE))
    //     vTaskDelay(pdMS_TO_TICKS(1));

    esp_lcd_panel_draw_bitmap(lcd_panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    // xQueueReceive(lcd_flush_done_queue, &flush_ready_flag, portMAX_DELAY); // yield here

    lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

static void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint8_t touch_signal;
    /*`touchpad_is_pressed` and `touchpad_get_xy` needs to be implemented by you*/
    if (xQueueReceive(touch_signal_queue, &touch_signal, 0) == pdTRUE)
    {
        gt911_read_pos(touch_handle);
        if (touch_handle->point_num && touch_handle->point_num <= 5)
        {
            data->state = LV_INDEV_STATE_PRESSED;
            data->point.x = touch_handle->point1_pos_x;
            data->point.y = touch_handle->point1_pos_y;
        }
        else
            return;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void lv_tick_task(void *arg)
{
    lv_tick_inc(LV_TICK_PERIOD_MS);
}
