//!
//! @file basic_test_routine.cpp
//!
//! @brief This file contains the basic test routine task implementation.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!


//! C++ standard library
#include <stdint.h>           
#include <stdio.h>

//! External Libraries
#include "pico/stdlib.h"
#include "hardware/uart.h"    //!< Raspberry Pi Pico SDK
#include "hardware/gpio.h" 
#include "projdefs.h"         //!< FreeRTOS

//! Project-specific Headers
#include "test_routine_2.h"

/* #define UART_ID uart0 */
/* #define BAUD_RATE 115200 */
/* #define UART_TX_PIN 0 */
/* #define UART_RX_PIN 1 */

TaskHandle_t xTaskTestRoutine2Handle;

void vTaskTestRoutine2(void *pvParameters) {
    stdio_init_all();

    ads1256_t ads;
    ads1256_init(&ads,
                 APP_ADS1256_DRDY_PIN,
                 APP_ADS1256_RESET_PIN,
                 APP_ADS1256_SYNC_PIN,
                 APP_ADS1256_CS_PIN,
                 APP_ADS1256_MISO_PIN,
                 APP_ADS1256_SCLK_PIN,
                 APP_ADS1256_MOSI_PIN,
                 APP_ADS1256_VREF);
    
    scanf("Wait...");
    printf("Starting:\n");

    ads1256_initialize(&ads);
    printf("ADS1256 initialized...\n");
    
    while (true) {
        ads1256_wait_drdy(&ads);
        ads1256_set_mux(&ads, ADS1256_MUX_DIFF_2_3);

        ads1256_start_communication(&ads);
        ads1256_sync(&ads);
        ads1256_wakeup(&ads);
        int32_t ids_raw = ads1256_read_single(&ads);
        ads1256_end_communication(&ads);
        
        ads1256_wait_drdy(&ads);
        ads1256_set_mux(&ads, ADS1256_MUX_DIFF_0_1);

        ads1256_start_communication(&ads);
        ads1256_sync(&ads);
        ads1256_wakeup(&ads);
        int32_t vgs_raw = ads1256_read_single(&ads);
        ads1256_end_communication(&ads);

        // Conversions
        if ((ids_raw >> 23) == 1) {
            ids_raw = ids_raw - 16777216;
        }
        float ids_v = (((float)(ids_raw-8888)) / 8388607.0) * 0.078125;
        float ids = ids_v/0.0876;

        if ((vgs_raw >> 23) == 1) {
            vgs_raw = vgs_raw - 16777216;
        }
        float vgs_v = (((float)(vgs_raw-200000)) / 8388607.0) * 0.078125;
        float vgs = (vgs_v*23928.0)/100.0;

        char buffer[64];
        uint32_t timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        snprintf(buffer, sizeof(buffer), "%ld;%0.9f;%0.9f\n", timestamp, ids, vgs);
        printf(buffer);
        vTaskDelay(TASK_TEST_ROUTINE_2_PERIOD_MS);
    }    
    // Should never reache here
};
