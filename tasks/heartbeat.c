/**
@file heartbeat.c
@brief Source file for the Rasp's heartbeat.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 08/04/2026
*/

#include <stdint.h>           

#include "projdefs.h"

#include "drivers/gpio_module.h"

#include "heartbeat.h"

TaskHandle_t xTaskHeartbeatHandle;

void vTaskHeartbeat(void *pvParameters) {

    gpio_init_pin(HEARTBEAT);
    gpio_set_pin_direction(HEARTBEAT, GPIO_OUTPUT);
    gpio_set_pin_state(HEARTBEAT, GPIO_HIGH);

    while(true) {
        gpio_toggle_pin(HEARTBEAT);
        vTaskDelay(TASK_HEARTBEAT_PERIOD_MS);
    }
};
