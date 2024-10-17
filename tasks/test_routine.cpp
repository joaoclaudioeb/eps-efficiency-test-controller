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
#include <cstdint>           

//! External Libraries
#include "hardware/uart.h"    //!< Raspberry Pi Pico SDK
#include "projdefs.h"         //!< FreeRTOS

//! Project-specific Drivers
#include "drivers/uart/uart_module.h"
#include "drivers/gpio_module.h"
#include "drivers/pwm_module.h"

//! Project-specific Headers
#include "test_routine.hpp"

TaskHandle_t xTaskTestRoutineHandle;

void vTaskTestRoutine(void *pvParameters) {

    //! Configure UART 
    drivers::UartConfig uart_cfg {
        .port = drivers::UartPort::UART_0,
    };
    drivers::UartController uart{uart_cfg};

    //! Configure GPIO
    GPIO gpio;
    
    gpio.initPin(EN_5V0_CHN_A);
    gpio.initPin(EN_3V3_CHN_A);
    gpio.initPin(EN_5V0_CHN_B);
    gpio.initPin(EN_3V3_CHN_B);
    gpio.initPin(EN_OUTPUT_CHN_0);
    gpio.initPin(EN_OUTPUT_CHN_1);
    gpio.initPin(EN_OUTPUT_CHN_2);
    gpio.initPin(EN_BQ_OUTPUT_CHN_A);
    gpio.initPin(EN_BQ_OUTPUT_CHN_B);
    gpio.initPin(TRIGGER_MEASUREMENT);
    
    gpio.setDirection(EN_3V3_CHN_A, gpio_dir::OUTPUT);
    gpio.setDirection(EN_5V0_CHN_A, gpio_dir::OUTPUT); 
    gpio.setDirection(EN_5V0_CHN_B, gpio_dir::OUTPUT);
    gpio.setDirection(EN_3V3_CHN_B, gpio_dir::OUTPUT);
    gpio.setDirection(EN_OUTPUT_CHN_0, gpio_dir::OUTPUT);
    gpio.setDirection(EN_OUTPUT_CHN_1, gpio_dir::OUTPUT);
    gpio.setDirection(EN_OUTPUT_CHN_2, gpio_dir::OUTPUT);
    gpio.setDirection(EN_BQ_OUTPUT_CHN_A, gpio_dir::OUTPUT);
    gpio.setDirection(EN_BQ_OUTPUT_CHN_B, gpio_dir::OUTPUT);
    gpio.setDirection(TRIGGER_MEASUREMENT, gpio_dir::OUTPUT);
    
    gpio.setState(EN_3V3_CHN_A, gpio_state::HIGH);
    gpio.setState(EN_5V0_CHN_A, gpio_state::HIGH);
    gpio.setState(EN_3V3_CHN_B, gpio_state::HIGH);
    gpio.setState(EN_5V0_CHN_B, gpio_state::HIGH);
    gpio.setState(EN_BQ_OUTPUT_CHN_A, gpio_state::HIGH);
    gpio.setState(EN_BQ_OUTPUT_CHN_B, gpio_state::HIGH);
    gpio.setState(TRIGGER_MEASUREMENT, gpio_state::LOW);

    unsigned char cmd, lock;
    // uint8_t lookup_table[22] = {1, 2, 3, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
    uint8_t lookup_table[15] = {1, 2, 3, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
    
    while (true) {
        // Select channel
        uart.flush();
        
        uart.puts("Select channel: \n");
        uart.puts("    A - 3V3 and 5V0 DC-DC converters of channel A\n");
        uart.puts("    B - 3V3 and 5V0 DC-DC converters of channel B\n");
        uart.puts("    C - MPPT-based converter of channel A\n");
        uart.puts("    D - MPPT-based converter of channel B\n");
        uart.puts("    E - ''Debug mode''\n");
        
        uart.read(&cmd, 1);

        int state = 0;

        // Activate correct channel
        switch (cmd) {
        case 'A':
            gpio.setState(EN_3V3_CHN_A, gpio_state::LOW);
            gpio.setState(EN_5V0_CHN_A, gpio_state::LOW);
            gpio.setState(EN_3V3_CHN_B, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_B, gpio_state::HIGH);
            state = 0;
            break;
        case 'B':
            gpio.setState(EN_3V3_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_3V3_CHN_B, gpio_state::LOW);
            gpio.setState(EN_5V0_CHN_B, gpio_state::LOW);
            state = 0;
            break;
        case 'E':
            gpio.setState(EN_3V3_CHN_A, gpio_state::LOW);
            gpio.setState(EN_5V0_CHN_A, gpio_state::LOW);
            gpio.setState(EN_3V3_CHN_B, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_B, gpio_state::HIGH);
            state = 1;
            break;
        case 'C':
            gpio.setState(EN_3V3_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_3V3_CHN_B, gpio_state::LOW);
            gpio.setState(EN_5V0_CHN_B, gpio_state::LOW);
            gpio.setState(EN_BQ_OUTPUT_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_BQ_OUTPUT_CHN_B, gpio_state::LOW);
            state = 2;
            break;
        case 'D':
            gpio.setState(EN_3V3_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_3V3_CHN_B, gpio_state::LOW);
            gpio.setState(EN_5V0_CHN_B, gpio_state::LOW);
            gpio.setState(EN_BQ_OUTPUT_CHN_A, gpio_state::LOW);
            gpio.setState(EN_BQ_OUTPUT_CHN_B, gpio_state::HIGH);
            state = 2;
            break;
        default:
            continue;
        }

        // Init the test
        uart.puts("Press any key to run test!\n");
        uart.read(&cmd, 1);

        
        gpio.setState(EN_OUTPUT_CHN_0, gpio_state::HIGH);
        gpio.setState(EN_OUTPUT_CHN_1, gpio_state::HIGH);
        gpio.setState(EN_OUTPUT_CHN_2, gpio_state::HIGH);
        
        pwm_config_t cfg = {
            .pin = PWM_CONTROLLED_LOAD, // P08 is the 
            .duty_cycle = 0,
            .frequency = 125000,
        };

        PWM pwm;
        pwm.configPwmPin(cfg);
        pwm.enablePwmPin(cfg, pwm_state::ENABLE);
        
        switch (state) {
        case 0: // This block will make the control of the LTC3833 converters, which is the output conveter that generates the 5V and 3V3 of the satellite. 
            // Run
            uart.puts("Starting test\n");
            for (uint8_t i = 0; i < ARRAY_SIZE(lookup_table); i++) {
                cfg.duty_cycle = lookup_table[i];
                pwm.setDutyCycle(cfg);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                for (uint8_t j = 0; j < 30; j++) {
                    vTaskDelay(120 / portTICK_PERIOD_MS);
                    gpio.setState(TRIGGER_MEASUREMENT, gpio_state::HIGH); // P05 is the signal pin for controlling the multimeter
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                    gpio.setState(TRIGGER_MEASUREMENT, gpio_state::LOW);
                }
                uart.puts("Duty cycle: ");
                uart.write(&lookup_table[i], 1);
                uart.puts("\n");
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            cfg.duty_cycle = 0;
            pwm.setDutyCycle(cfg);

            gpio.setState(EN_3V3_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_A, gpio_state::HIGH);
            gpio.setState(EN_3V3_CHN_B, gpio_state::HIGH);
            gpio.setState(EN_5V0_CHN_B, gpio_state::HIGH);
            gpio.setState(EN_OUTPUT_CHN_0, gpio_state::LOW);
            gpio.setState(EN_OUTPUT_CHN_1, gpio_state::LOW);
            gpio.setState(EN_OUTPUT_CHN_2, gpio_state::LOW);
            uart.puts("Finished test\n");
            break;
        case 1: // This block is used only for debug purpuses
            cfg.duty_cycle = 80;
            pwm.setDutyCycle(cfg);
            uart.puts("End the debug? \n");
            uart.read(&cmd, 1);
            cfg.duty_cycle = 0;
            pwm.setDutyCycle(cfg);
            break;
        case 2: // This block will make the control of the BQ24650, which is the MPPT-based converter. 
            lock = 'Y';
            uart.puts("Press 'S' to run measurements!\n");
            uart.read(&cmd, 1);
            while (lock == 'Y'){
                if (cmd == 'S'){
                    for (uint8_t j = 0; j < 50; j++) {
                        vTaskDelay(120 / portTICK_PERIOD_MS);
                        gpio.setState(TRIGGER_MEASUREMENT, gpio_state::HIGH);
                        vTaskDelay(10 / portTICK_PERIOD_MS);
                        gpio.setState(TRIGGER_MEASUREMENT, gpio_state::LOW);
                    }
                }
                uart.puts("Do you want to make more tests? (Y, N) \n");
                uart.read(&lock, 1);
            }
            break;
        default:
            continue;
        }
    }
};
