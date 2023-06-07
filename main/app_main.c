#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "iic.h"
#include "app.h"

void app_main(void)
{
    i2c_master_init();

    app_display_start();
}
