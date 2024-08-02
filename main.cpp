/**
 * @file main.cpp
 *
 * @brief This file contains the main(), to group the .h and .cpp files to run the application.
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 *
 * @date 29/07/2024
 */

#include <FreeRTOS.h>
#include <cstdint>
#include <task.h>
#include <stdio.h>

#include "drivers/uart/uart_module.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "tasks/tasks.h"

int main(void)
{
    // Creates the tasks
    // create_tasks();
    
    // Starts the scheduler
    // vTaskStartScheduler();
    drivers::UartConfig uart_cfg {
        .port = drivers::UartPort::UART_0,
    };

    drivers::UartController uart{uart_cfg};

    while(1) {
        uint8_t a[] = {0x65, 0x65};
        uart.write(a, 2);

        sleep_ms(500U);
    }

    // Should never reach it
    return 0;
}

