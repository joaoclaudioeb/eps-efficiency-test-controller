/**
@file dac81408_test_routine.h
@brief Header file for testing the TI's 16-bit DAC, the DAC81408.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 08/04/2026
*/

#ifndef DAC81408_TEST_ROUTINE_H_
#define DAC81408_TEST_ROUTINE_H_

#include <FreeRTOS.h>
#include <task.h>
#include "device-drivers/dac81408/dac81408.h"

#define TASK_DAC81408_TEST_ROUTINE_NAME                 "DAC81408 test"
#define TASK_DAC81408_TEST_ROUTINE_STACK_SIZE           2048
#define TASK_DAC81408_TEST_ROUTINE_PRIORITY             5
#define TASK_DAC81408_TEST_ROUTINE_PERIOD_MS            1000
#define TASK_DAC81408_TEST_ROUTINE_CORE                 (( 1 << 1 ) | ( 1 << 0 )) //!< Can run in both cores

#define APP_DAC81408_LDAC_PIN  16
#define APP_DAC81408_RESET_PIN 17
#define APP_DAC81408_CS_PIN    13
#define APP_DAC81408_MISO_PIN  12
#define APP_DAC81408_SCLK_PIN  10
#define APP_DAC81408_MOSI_PIN  11

extern TaskHandle_t xTaskDac81408TestRoutineHandle;

void vTaskDac81408TestRoutine(void *pvParameters);

/* DAC81408_TEST_ROUTINE_H_ */
#endif 
