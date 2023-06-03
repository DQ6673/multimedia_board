#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"

#include "iic.h"
#include "gt911.h"
#include "app.h"

const static char *TAG = "app_main";
extern EventGroupHandle_t gt911_event_group;

void app_main(void)
{
    i2c_master_init();
    app_display_start();
    
    gt911_handle_t gt911_handle = NULL;
    gt911_handle = gt911_new_dev();
    gt911_handle->init();
    gt911_handle->read_cfg();
    gt911_handle->swap_x_y();
    gt911_handle->mirror_x();

    for (;;)
    {
        // gt911 wait touch
        xEventGroupWaitBits(gt911_event_group,
                            GT911_EVENT_TOUCH,
                            pdTRUE, pdTRUE,
                            portMAX_DELAY); // yield here until touch

        gt911_read_pos(gt911_handle);

        if (gt911_handle->point_num && gt911_handle->point_num <= 5)
        {
            /* some process */
        }

        // using delay to feed watchdog
        // vTaskDelay(pdMS_TO_TICKS(1));
    }
}
