/**
 * @file main.cpp
 *
 * @brief TODO.
 *
 * @details TODO. 
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 * @date 29/07/2024
 */

// TODO: Add headers here
#include "tasks/basic_test_routine.h"

int main(void)
{
    // TODO: Add some code here
    xTaskCreate(basicTestRoutineTask, TASK_BASIC_TEST_ROUTINE_TASK_NAME, TASK_BASIC_TEST_ROUTINE_TASK_STACK_SIZE, nullptr, TASK_BASIC_TEST_ROUTINE_TASK_PRIORITY, nullptr);
    vTaskStartScheduler();
    return 0;
}

