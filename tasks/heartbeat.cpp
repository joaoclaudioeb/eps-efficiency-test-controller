//!
//! @file heartbeat.cpp
//!
//! @brief This file contains the heartbeat task implementation.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!


//! C++ standard library
#include <cstdint>           

//! External Libraries
#include "projdefs.h"         //!< FreeRTOS

//! Project-specific Drivers
#include "drivers/gpio/gpio_module.hpp"

//! Project-specific Headers
#include "heartbeat.hpp"

TaskHandle_t xTaskHeartbeatHandle;

void vTaskHeartbeat(void *pvParameters) {

    //! Configure GPIO
    drivers::GpioController gpio;
    
    gpio.init(HEARTBEAT);
    gpio.dir(HEARTBEAT, drivers::gpio_dir::OUTPUT);
    gpio.set(HEARTBEAT, drivers::gpio_state::LOW);

    while(true) {
        gpio.toggle(HEARTBEAT);
        vTaskDelay(TASK_HEARTBEAT_PERIOD_MS);
    }
};
