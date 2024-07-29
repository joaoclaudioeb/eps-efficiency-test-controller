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

void main(void)
{
   // Creates the tasks
   create_tasks();

   // Starts the scheduler
   vTaskStartScheduler();

  // Should never reach it
  while(1);
}

