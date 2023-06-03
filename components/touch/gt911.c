#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_check.h"

#include "iic.h"
#include "gt911.h"

const static char *TAG = "gt911";

#define GT911_RESET_PIN 35
#define GT911_INT_PIN 18

#define GT911_DEV_ADDR_1 0X28
#define GT911_DEV_ADDR_2 0XBA
#define GT911_DEV_ADDR GT911_DEV_ADDR_2

bool swap_x_y_set = false;
bool mirror_x_set = false;
bool mirror_y_set = false;

#define GT911_RES_V 480
#define GT911_RES_H 320
#define GT911_TP_NUM 5
// const uint8_t gt911_cfg_param[] = {
//     0x61, (GT911_RES_H % 256), (GT911_RES_H / 256), (GT911_RES_V % 256), (GT911_RES_V / 256), GT911_TP_NUM, 0x0D, 0x00, 0x01, 0x08,
//     0x28, 0x09, 0x46, 0x3C, 0x0F, 0x05, 0x01, 0x01, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x15, 0x17, 0x19, 0x14, 0x87, 0x28, 0x0A,
//     0x48, 0x46, 0x46, 0x08, 0x00, 0x00, 0x00, 0xBA, 0x02, 0x1D,
//     0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x46, 0x8C, 0x94, 0xC5, 0x02, 0x07, 0x00, 0x00, 0x04,
//     0x7B, 0x4B, 0x00, 0x73, 0x56, 0x00, 0x6C, 0x63, 0x00, 0x67,
//     0x71, 0x00, 0x63, 0x82, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,
//     0x12, 0x14, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x1D,
//     0x1E, 0x1F, 0x20, 0x21, 0x22, 0x24, 0x26, 0xFF, 0xFF, 0xFF,
//     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x73, 0x01};

const uint8_t gt911_cfg_param_factory[] = {
    0X61, 0X40, 0X1, 0XE0, 0X1, 0X5, 0X35, 0, 0X2, 0X8, 0X1E,
    0X8, 0X50, 0X3C, 0XF, 0X5, 0, 0, 0, 0, 0X50,
    0, 0, 0X18, 0X1A, 0X1E, 0X14, 0X87, 0X27, 0XA, 0X4B,
    0X4D, 0XD3, 0X7, 0, 0, 0, 0X2, 0X32, 0X1D, 0,
    0, 0, 0, 0, 0, 0, 0X32, 0, 0, 0X2A,
    0X32, 0X64, 0X94, 0XD5, 0X2, 0X7, 0, 0, 0X4, 0XA5,
    0X35, 0, 0X91, 0X3D, 0, 0X80, 0X46, 0, 0X70, 0X51,
    0, 0X63, 0X5D, 0, 0X63, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0X14, 0X12, 0X10, 0XE, 0XC, 0XA, 0X8, 0X6, 0X4,
    0X2, 0XFF, 0XFF, 0XFF, 0XFF, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0X2, 0X4, 0X6, 0X8, 0XA, 0XC, 0X24, 0X22,
    0X21, 0X20, 0X1F, 0X1E, 0X1D, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0X21, 0};

EventGroupHandle_t gt911_event_group = NULL;

/*************************************************************/
static void gt911_init(void);
static void gt911_set_dev_addr(void);
static void gt911_read_reg(uint16_t reg_addr, uint8_t *datbuf, size_t dat_len);
static void gt911_write_reg(uint16_t reg_addr, uint8_t *datbuf, size_t dat_len);
static void gt911_config(void);
static void gt911_read_cfg(void);
static void IRAM_ATTR gt911_int_handler(void *arg);
static void gt911_swap_x_y(void);
static void gt911_mirror_x(void);
static void gt911_mirror_y(void);
/*************************************************************/

gt911_handle_t gt911_new_dev(void)
{
    gt911_handle_t ret_handle = NULL;
    ret_handle = malloc(sizeof(gt911_t));
    if (ret_handle == NULL)
    {
        ESP_LOGE(TAG, "cannot create device handle");
        return ret_handle;
    }

    ret_handle->init = gt911_init;
    ret_handle->read_cfg = gt911_read_cfg;
    ret_handle->write_cfg = gt911_config;
    ret_handle->swap_x_y = gt911_swap_x_y;
    ret_handle->mirror_x = gt911_mirror_x;
    ret_handle->mirror_y = gt911_mirror_y;

    return ret_handle;
}

static void gt911_init(void)
{
    gt911_set_dev_addr();
    // gt911_config();
    // gt911_read_cfg();
    gt911_event_group = xEventGroupCreate();
    if (gt911_event_group == NULL)
        ESP_LOGE(TAG, "event group created,faild");
}

static void gt911_set_dev_addr(void)
{
    gpio_config_t io_conf = {
        // reset 和 int 设置为输出
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = ((1ULL << GT911_RESET_PIN) | (1ULL << GT911_INT_PIN)),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    gpio_set_level(GT911_RESET_PIN, 0);
    gpio_set_level(GT911_INT_PIN, 0);

    if (GT911_DEV_ADDR == GT911_DEV_ADDR_1) // 0X28
    {
        gpio_set_level(GT911_INT_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1));
        gpio_set_level(GT911_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(GT911_INT_PIN, 0);
    }
    else if (GT911_DEV_ADDR == GT911_DEV_ADDR_2) // 0XBA
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        gpio_set_level(GT911_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(60));

    io_conf.intr_type = GPIO_INTR_NEGEDGE; // INT 引脚改为浮空输入
    io_conf.pin_bit_mask = (1ULL << GT911_INT_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GT911_INT_PIN, gt911_int_handler, NULL);
}

static void gt911_write_reg(uint16_t reg_addr, uint8_t *datbuf, size_t dat_len)
{
    size_t trans_len = dat_len + 2;
    uint8_t trans_buf[200] = {
        (reg_addr / 256),
        (reg_addr % 256),
    };

    for (int i = 0; i < dat_len; i++)
    {
        trans_buf[i + 2] = *(datbuf + i);
    }

    ESP_ERROR_CHECK(i2c_send(GT911_DEV_ADDR, trans_buf, trans_len));
}

static void gt911_read_reg(uint16_t reg_addr, uint8_t *datbuf, size_t dat_len)
{
    size_t trans_len = 2;
    uint8_t trans_buf[200] = {
        (reg_addr / 256),
        (reg_addr % 256),
    };
    ESP_ERROR_CHECK(i2c_send(GT911_DEV_ADDR, trans_buf, trans_len));

    ESP_ERROR_CHECK(i2c_read(GT911_DEV_ADDR, datbuf, dat_len));
}

static void gt911_read_cfg(void)
{
    size_t ReadLen = 186;
    uint8_t ReadConfig[200] = {};

    gt911_read_reg(GTP_CONFIG_VESION, ReadConfig, ReadLen);
    ESP_LOGW(TAG, "ReadConfigReg:%#X", GTP_CONFIG_VESION);
    for (int i = 0; i < ReadLen; i++)
    {
        printf("%#X, ", ReadConfig[i]);
        if (i % 10 == 0 && i)
            printf("\r\n");
    }
    printf("\r\n");
}

static void gt911_config(void)
{
    gt911_write_reg(GTP_CONFIG_VESION, gt911_cfg_param_factory, sizeof(gt911_cfg_param_factory));
}

void gt911_read_pos(gt911_handle_t handle)
{
    uint8_t status_reg = 0, count = 0;
    uint8_t pos_read[40] = {0};

    gt911_read_reg(GTP_STATUS_RW, &status_reg, 1); // read buffer status
    while ((status_reg & 0X80) == 0)               // if buffer status bit 7 = 0
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        gt911_read_reg(GTP_STATUS_RW, &status_reg, 1); // re-read per 1ms
        if (++count >= 10)
        {
            ESP_LOGE(TAG, "read pos failed");
            return;
        }
    }
    // ESP_LOGI(TAG, "status:%#X", status_reg);
    if (status_reg & 0X0F) // point_num != 0
    {
        handle->point_num = status_reg & 0X0F;

        gt911_read_reg(0X814F, pos_read, 40); // read position info
        // for (int i = 0; i < 40; i++)
        //     printf("%#X ", pos_read[i]);
        // printf("\r\n");
        handle->point1_track_id = pos_read[0];
        handle->point1_size = pos_read[5] + pos_read[6] * 256;
        if (swap_x_y_set)
        {
            handle->point1_pos_y = mirror_y_set ? pos_read[1] + (pos_read[2] << 8) : GT911_RES_H - (pos_read[1] + (pos_read[2] << 8));
            handle->point1_pos_x = mirror_x_set ? pos_read[3] + (pos_read[4] << 8) : GT911_RES_V - (pos_read[3] + (pos_read[4] << 8));
        }
        else
        {
            handle->point1_pos_x = mirror_x_set ? pos_read[1] + (pos_read[2] << 8) : GT911_RES_H - (pos_read[1] + (pos_read[2] << 8));
            handle->point1_pos_y = mirror_y_set ? pos_read[3] + (pos_read[4] << 8) : GT911_RES_V - (pos_read[3] + (pos_read[4] << 8));
        }
    }

    status_reg = 0;
    gt911_write_reg(GTP_STATUS_RW, &status_reg, 1); // clear status buffer
}

static void IRAM_ATTR gt911_int_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(
        gt911_event_group,
        GT911_EVENT_TOUCH,
        &xHigherPriorityTaskWoken);
}

static void gt911_swap_x_y(void)
{
    swap_x_y_set = true;
}

static void gt911_mirror_x(void)
{
    mirror_x_set = true;
}

static void gt911_mirror_y(void)
{
    mirror_y_set = true;
}
