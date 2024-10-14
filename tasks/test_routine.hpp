//!
//! @file basic_test_routine.h
//!
//! @brief This file contains the basic test routine task header.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @author João Cláudio Elsen Barcellos  <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

#ifndef TEST_ROUTINE_HPP_
#define TEST_ROUTINE_HPP_

//! External Libraries
#include <FreeRTOS.h>    //!< FreeRTOS
#include <task.h>        //!< FreeRTOS

//! Task properties
#define TASK_TEST_ROUTINE_NAME                 "Basic Test Routine"
#define TASK_TEST_ROUTINE_STACK_SIZE           2048
#define TASK_TEST_ROUTINE_PRIORITY             5
#define TASK_TEST_ROUTINE_PERIOD_MS            500
#define TASK_TEST_ROUTINE_CORE                 (( 1 << 1 ) | ( 1 << 0 )) //!< Can run in both cores

//! Useful macros
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

//! Task-related macros
#define EN_5V0_CHN_A           drivers::gpio_id::P21
#define EN_3V3_CHN_A           drivers::gpio_id::P20
#define EN_5V0_CHN_B           drivers::gpio_id::P19
#define EN_3V3_CHN_B           drivers::gpio_id::P18
#define EN_OUTPUT_CHN_0        drivers::gpio_id::P13 //!< RadioMain_EN
#define EN_OUTPUT_CHN_1        drivers::gpio_id::P14 //!< RadioBeacon_EN
#define EN_OUTPUT_CHN_2        drivers::gpio_id::P15 //!< Payload_EN
//#define EN_OUTPUT_CHN_3       drivers::gpio_id::Pxx //!< AntDep_EN
//#define EN_OUTPUT_CHN_4       drivers::gpio_id::Pxx //!< Beacon_EN
//#define EN_OUTPUT_CHN_5       drivers::gpio_id::Pxx //!< OBDH_EN
#define EN_BQ_OUTPUT_CHN_A     drivers::gpio_id::P27
#define EN_BQ_OUTPUT_CHN_B     drivers::gpio_id::P26
#define TRIGGER_MEASUREMENT    drivers::gpio_id::P05
#define PWM_CONTROLLED_LOAD    drivers::gpio_id::P08

//!
//! @brief Heartbeat task handle.
//!
extern TaskHandle_t xTaskTestRoutineHandle;

//!
//! @brief Basic test routine task
//! 
//! @param pvParameters Not used
//!
void vTaskTestRoutine(void *pvParameters);

//! TEST_ROUTINE_HPP_
#endif 
