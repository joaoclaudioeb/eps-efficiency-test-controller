//!
//! @file re2ps_read_routine.h
//!
//! @brief This file contains the basic test routine task header.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @author João Cláudio Elsen Barcellos  <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

#ifndef RE2PS_READ_ROUTINE_HPP_
#define RE2PS_READ_ROUTINE_HPP_

//! External Libraries
#include <FreeRTOS.h>    //!< FreeRTOS
#include <task.h>        //!< FreeRTOS

//! Task properties
#define TASK_RE2PS_READ_ROUTINE_NAME                 "RE2PS Read Routine"
#define TASK_RE2PS_READ_ROUTINE_STACK_SIZE           2048
#define TASK_RE2PS_READ_ROUTINE_PRIORITY             5
#define TASK_RE2PS_READ_ROUTINE_PERIOD_MS            1000 / portTICK_PERIOD_MS
#define TASK_RE2PS_READ_ROUTINE_CORE                 ( 1 << 0 ) //!< Run only in core 0

//! Task-related macros
#define TOGGLE_MUX_CHN      gpio_id::P25
#define MUX_BIT_0           gpio_id::P20
#define MUX_BIT_1           gpio_id::P19
#define MUX_BIT_2           gpio_id::P18
#define READ_ADC            gpio_id::P13 

//!
//! @brief Heartbeat task handle.
//!
extern TaskHandle_t xTaskRe2psReadRoutineHandle;

//!
//! @brief Basic test routine task
//! 
//! @param pvParameters Not used
//!
void vTaskRe2psReadRoutine(void *pvParameters);

//! RE2PS_READ_ROUTINE_HPP_
#endif 
