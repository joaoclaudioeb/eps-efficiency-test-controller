//!
//! @file basic_test_routine.h
//!
//! @brief This file contains the basic test routine task header.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @author João Cláudio Elsen Barcellos  <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

#ifndef TEST_ROUTINE_2_HPP_
#define TEST_ROUTINE_2_HPP_

//! External Libraries
#include <FreeRTOS.h>    //!< FreeRTOS
#include <task.h>        //!< FreeRTOS
#include "device-drivers/ads1256/ads1256.h"

//! Task properties
#define TASK_TEST_ROUTINE_2_NAME                 "ADS1256 test"
#define TASK_TEST_ROUTINE_2_STACK_SIZE           2048
#define TASK_TEST_ROUTINE_2_PRIORITY             5
#define TASK_TEST_ROUTINE_2_PERIOD_MS            5000
#define TASK_TEST_ROUTINE_2_CORE                 (( 1 << 1 ) | ( 1 << 0 )) //!< Can run in both cores

// Pinos do Raspberry Pi Pico conectados ao ADS1256
#define APP_ADS1256_DRDY_PIN  16   // GPIO15 - DRDY (Data Ready)
#define APP_ADS1256_RESET_PIN 17   // GPIO14 - RESET (0 = não usado)
#define APP_ADS1256_SYNC_PIN  0    // 0 = não usado
#define APP_ADS1256_CS_PIN    13   // GPIO13 - Chip Select
#define APP_ADS1256_MISO_PIN  12   // GPIO12 - MISO (SPI0)
#define APP_ADS1256_SCLK_PIN  10   // GPIO10 - SCLK (SPI0)
#define APP_ADS1256_MOSI_PIN  11   // GPIO11 - MOSI (SPI0)

// Configurações do ADC para ESTA aplicação
#define APP_ADS1256_VREF          2.5f                          // Tensão de referência (V)
#define APP_ADS1256_PGA_CONFIG    ADS1256_PGA_64                // Ganho: 16x
#define APP_ADS1256_DRATE_CONFIG  ADS1256_DRATE_2000SPS          // Taxa: 100 SPS
#define APP_ADS1256_BUFFER_EN     ADS1256_BUFFER_ENABLED        // Buffer: habilitado

//!
//! @brief Heartbeat task handle.
//!
extern TaskHandle_t xTaskTestRoutine2Handle;

//!
//! @brief Basic test routine task
//! 
//! @param pvParameters Not used
//!
void vTaskTestRoutine2(void *pvParameters);

//! TEST_ROUTINE_2_HPP_
#endif 
