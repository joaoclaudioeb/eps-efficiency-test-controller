//!
//! @file main.cpp
//!
//! @brief This file creates all the project's tasks.
//!
//! @author João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

//! External Libraries
#include <FreeRTOS.h>    //!< FreeRTOS
#include <task.h>        //!< FreeRTOS

#include "pico/stdlib.h"

//! Project-specific Headers
#include "config/tasks.h"

int main(void)
{
    stdio_init_all();

    create_tasks();

    vTaskStartScheduler();

    // Should never reache here
    return 0;
}

