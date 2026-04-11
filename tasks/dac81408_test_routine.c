/**
@file dac81408_test_routine.c
@brief Source file for testing the TI's 16-bit DAC, the DAC81408.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 08/04/2026
*/

#include <stdint.h>           
#include <stdio.h>

#include "pico/stdlib.h"
/* #include "hardware/uart.h" */
#include "hardware/gpio.h" 
#include "projdefs.h"

#include "dac81408_test_routine.h"

TaskHandle_t xTaskDac81408TestRoutineHandle;

void vTaskDac81408TestRoutine(void *pvParameters) {
    dac81408_t dac;
    dac81408_init(&dac,
                 APP_DAC81408_LDAC_PIN,
                 APP_DAC81408_RESET_PIN,
                 APP_DAC81408_CS_PIN,
                 APP_DAC81408_MISO_PIN,
                 APP_DAC81408_SCLK_PIN,
                 APP_DAC81408_MOSI_PIN);

    scanf("Wait...");
    printf("Starting DAC81408 test...\n");

    // Loop de inicialização com timeout opcional
    int retries = 0;
    while(dac81408_initialize(&dac) != 0) {

        uint16_t val = dac81408_read_register(&dac, DAC81408_REG_DEVICEID);
        printf("DEVICEID: 0x%04X\n", val);

        uint16_t cfg = dac81408_read_register(&dac, DAC81408_REG_SPICONFIG);
        printf("SPICONFIG: 0x%04X\n", cfg);
    
        printf("DAC init failed, retry %d...\n", ++retries);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        if(retries > 10) {
            printf("ERROR: Too many retries, aborting\n");
            vTaskDelete(NULL);  // Encerra a task
            return;
        }
    }
    printf("DAC initialized OK\n");

    dac81408_set_int_reference(&dac, DAC81408_REF_ON);
    dac81408_set_ch_enabled(&dac, 7, true);
    dac81408_set_range(&dac, 7, DAC81408_RANGE_0_5V);
    dac81408_set_out(&dac, 7, 0);
    printf("Channel 7 configured: 0-5V, starting ramp...\n");

    uint16_t i = 0;
    
    while (true) {
        
        dac81408_set_out(&dac, 7, (i)%65535);

        i = (i+64)%65535;
        
        vTaskDelay(TASK_DAC81408_TEST_ROUTINE_PERIOD_MS);
    }    
    // Should never reache here
};
