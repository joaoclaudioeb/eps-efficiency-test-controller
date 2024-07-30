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
#include <task.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "tasks/tasks.h"

#include "drivers/uart_module.cpp"

Serial serial(DEFAULT_UART_CHANNEL, DEFAULT_PARITY, DEFAULT_BAUD_RATE, DEFAULT_DATA_BITS, DEFAULT_STOP_BITS, DEFAULT_TX_PIN, DEFAULT_RX_PIN);

int main(void)
{
    // Creates the tasks
    // create_tasks();
    
    // Starts the scheduler
    // vTaskStartScheduler();
    
    serial.sendPackage("\nI'm alive!\n");
    
    while(1);

    // Should never reach it
    return 0;
}

