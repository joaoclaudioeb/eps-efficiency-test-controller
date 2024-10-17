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

//! Project-specific Headers
#include "config/tasks.hpp"

int main(void)
{
    //! Create all the prject's tasks
    create_tasks();
    //! Start the scheduler
    vTaskStartScheduler();
    //! Should never reach it
    return 0;
}

