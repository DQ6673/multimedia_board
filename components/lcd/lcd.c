#include "esp_lcd_panel_st7796.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "lcd.h"

//
#define LCD_BUS_WIDTH 8
#define LCD_PSRAM_DATA_ALIGNMENT 64
#define LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8
#define LCD_MAX_TRANS_BYTES (LCD_RES_H * LCD_RES_V * sizeof(uint16_t))

//
#define LCD_BITS_PER_PIXEL 16 // RGB565

esp_lcd_panel_handle_t lcd_panel_handle;
QueueHandle_t lcd_flush_done_queue;

static bool lcd_color_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // static uint8_t flush_done_flag = 1;

    // xQueueSendFromISR(lcd_flush_done_queue, &flush_done_flag, &xHigherPriorityTaskWoken);

    return false;
}

void lcd_init(void)
{
    gpio_config_t io_conf = {
        // Tear Effect
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << LCD_PIN_TE),
        .pull_down_en = 1,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    esp_lcd_i80_bus_handle_t i80_bus_handle = NULL;
    esp_lcd_i80_bus_config_t i80_bus_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT, // 160MHz
        .dc_gpio_num = LCD_PIN_DCX,
        .wr_gpio_num = LCD_PIN_WRX,
        .data_gpio_nums = {
            LCD_PIN_DB0,
            LCD_PIN_DB1,
            LCD_PIN_DB2,
            LCD_PIN_DB3,
            LCD_PIN_DB4,
            LCD_PIN_DB5,
            LCD_PIN_DB6,
            LCD_PIN_DB7,
        },
        .bus_width = LCD_BUS_WIDTH,
        .max_transfer_bytes = LCD_MAX_TRANS_BYTES,
        .psram_trans_align = LCD_PSRAM_DATA_ALIGNMENT,
        .sram_trans_align = 4,
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&i80_bus_config, &i80_bus_handle));

    esp_lcd_panel_io_handle_t io_handle = NULL; // 输入输出方法集
    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = LCD_PIN_CSX,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        // .flags = {
        //     .swap_color_bytes = !LV_COLOR_16_SWAP, // Swap can be done in LvGL (default) or DMA
        // },
        .on_color_trans_done = lcd_color_trans_done_cb,
        // .user_ctx = &disp_drv,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus_handle, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t lcd_panel_config = {
        .reset_gpio_num = LCD_PIN_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7796(io_handle, &lcd_panel_config, &lcd_panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(lcd_panel_handle, true));

    lcd_flush_done_queue = xQueueCreate(1, sizeof(uint8_t));
    // ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd_panel_handle, true));
    // ESP_ERROR_CHECK(esp_lcd_panel_set_gap(lcd_panel_handle, 0, 20));
    // ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, true));
}