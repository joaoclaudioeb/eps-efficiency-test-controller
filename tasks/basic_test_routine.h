/**
 * @file basic_test_routine.h
 *
 * @brief This file contains the basic test routine task header.
 *
 * @author Ramon de Araujo Borba <ramonborba97@gmail.com>
 * @version 0.0.0
 *
 * @date 06/08/2024
 */

#ifndef BASIC_TEST_ROUTINE_TASK_HPP_
#define BASIC_TEST_ROUTINE_TASK_HPP_

#include <FreeRTOS.h>
#include <task.h>

/* Task properties */
#define TASK_BASIC_TEST_ROUTINE_TASK_NAME                 "Basic Test Routine"
#define TASK_BASIC_TEST_ROUTINE_TASK_STACK_SIZE           2048
#define TASK_BASIC_TEST_ROUTINE_TASK_PRIORITY             5
#define TASK_BASIC_TEST_ROUTINE_TASK_PERIOD_MS            500
#define TASK_BASIC_TEST_ROUTINE_TASK_CORE                 0


/**
 * \brief Basic test routine task
 * 
 * \param pvParameters Not used
 */
void basicTestRoutineTask(void *pvParameters);

#endif /* BASIC_TEST_ROUTINE_TASK_HPP_ */
