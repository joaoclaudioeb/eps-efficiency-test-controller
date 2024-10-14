//!
//! @file heartbeat.hpp
//!
//! @brief This file contains the basic test routine task header.
//!
//! @author João Cláudio Elsen Barcellos  <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

#ifndef HEARTBEAT_HPP_
#define HEARTBEAT_HPP_

//! External Libraries
#include <FreeRTOS.h>    //!< FreeRTOS
#include <task.h>        //!< FreeRTOS

//! Task properties
#define TASK_HEARTBEAT_NAME                 "Heartbeat"
#define TASK_HEARTBEAT_STACK_SIZE           100
#define TASK_HEARTBEAT_PRIORITY             5
#define TASK_HEARTBEAT_PERIOD_MS            1000 / portTICK_PERIOD_MS
#define TASK_HEARTBEAT_CORE                 (( 1 << 1 ) | ( 1 << 0 )) //!< Can run in both cores

//! Task-related macros
#define HEARTBEAT           drivers::gpio_id::P25

//!
//! @brief Heartbeat task handle.
//!
extern TaskHandle_t xTaskHeartbeatHandle;

//!
//! \brief Basic heartbeat task
//! 
//! \param pvParameters Not used
//!
void vTaskHeartbeat(void *pvParameters);

//! HEARTBEAT_HPP_
#endif 
