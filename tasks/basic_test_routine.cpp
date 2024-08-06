/**
 * @file basic_test_routine.cpp
 *
 * @brief This file contains the basic test routine task implementation.
 *
 * @author Ramon de Araujo Borba <ramonborba97@gmail.com>
 * @version 0.0.0
 *
 * @date 06/08/2024
 */

#include "basic_test_routine.h"
#include "drivers/uart/uart_module.h"
#include "drivers/gpio_module.h"
#include "drivers/pwm_module.h"
#include "hardware/uart.h"
#include "projdefs.h"
#include <cstdint>


#define CHANNEL_5V0_A_PIN gpio_id::P02
#define CHANNEL_3V3_A_PIN gpio_id::P03
#define CHANNEL_5V0_B_PIN gpio_id::P04
#define CHANNEL_3V3_B_PIN gpio_id::P05


void basicTestRoutineTask(void *pvParameters) {

    drivers::UartConfig uart_cfg {
        .port = drivers::UartPort::UART_0,
    };

    drivers::UartController uart{uart_cfg};

    GPIO gpio;
    gpio.initPin(CHANNEL_5V0_A_PIN);
    gpio.initPin(CHANNEL_3V3_A_PIN);
    gpio.initPin(CHANNEL_5V0_B_PIN);
    gpio.initPin(CHANNEL_3V3_B_PIN);

    gpio.setState(CHANNEL_3V3_A_PIN, gpio_state::HIGH);
    gpio.setState(CHANNEL_5V0_A_PIN, gpio_state::HIGH);
    gpio.setState(CHANNEL_3V3_B_PIN, gpio_state::HIGH);
    gpio.setState(CHANNEL_5V0_B_PIN, gpio_state::HIGH);

    while (true) {
        // Select channel 
        uart.puts("Selec channel (A, B): \n");
        uint8_t cmd;
        // uart.read(&cmd, 1);
        cmd = uart_getc(uart0);

        // Activate correct channel
        switch (cmd) {
            case 'A':
                gpio.setState(CHANNEL_3V3_A_PIN, gpio_state::LOW);
                gpio.setState(CHANNEL_5V0_A_PIN, gpio_state::LOW);
                gpio.setState(CHANNEL_3V3_B_PIN, gpio_state::HIGH);
                gpio.setState(CHANNEL_5V0_B_PIN, gpio_state::HIGH);
            break;

            case 'B':
                gpio.setState(CHANNEL_3V3_A_PIN, gpio_state::HIGH);
                gpio.setState(CHANNEL_5V0_A_PIN, gpio_state::HIGH);
                gpio.setState(CHANNEL_3V3_B_PIN, gpio_state::LOW);
                gpio.setState(CHANNEL_5V0_B_PIN, gpio_state::LOW);
            break;
        }

        uart.puts("Press any key to run test!\n");
        // uart.read(&cmd, 1);
        cmd = uart_getc(uart0);

        pwm_config_t cfg = {
            .pin = gpio_id::P06,
            .duty_cycle = 0,
            .frequency = 100000,
        };

        PWM pwm;
        pwm.configPwmPin(cfg);
        pwm.enablePwmPin(cfg, pwm_state::ENABLE);

        // Run
        uart.puts("Starting test\n");
        for (uint8_t i = 5; i < 100; i+=5) {
            cfg.duty_cycle = i;
            pwm.setDutyCycle(cfg);
            uart.puts("Duty cycle: ");
            uart.write(&i, 1);
            uart.puts("\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        pwm.enablePwmPin(cfg, pwm_state::DISABLE);

        gpio.setState(CHANNEL_3V3_A_PIN, gpio_state::HIGH);
        gpio.setState(CHANNEL_5V0_A_PIN, gpio_state::HIGH);
        gpio.setState(CHANNEL_3V3_B_PIN, gpio_state::HIGH);
        gpio.setState(CHANNEL_5V0_B_PIN, gpio_state::HIGH);
        uart.puts("Finished test\n");
    }
};
