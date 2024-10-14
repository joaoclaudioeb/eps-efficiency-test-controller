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
#include "drivers/gpio_module.h"

//! Project-specific Headers
#include "heartbeat.hpp"

TaskHandle_t xTaskHeartbeatHandle;

void vTaskHeartbeat(void *pvParameters) {

    //! Configure GPIO
    GPIO gpio;
    
    gpio.initPin(HEARTBEAT);
    gpio.setDirection(HEARTBEAT, gpio_dir::OUTPUT);
    gpio.setState(HEARTBEAT, gpio_state::HIGH);

    while(true) {
        gpio.togglePin(HEARTBEAT);
        vTaskDelay(TASK_HEARTBEAT_PERIOD_MS);
    }
};
