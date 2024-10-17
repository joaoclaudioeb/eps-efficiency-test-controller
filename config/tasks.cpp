//!
//! @file tasks.cpp
//!
//! @brief This file creates all the project's tasks.
//!
//! @author João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!

//! Project-specific Drivers
#include "config.hpp"
#include "tasks.hpp"

//! Project-specific Headers
#include "../tasks/test_routine.hpp"
#include "../tasks/heartbeat.hpp"

void create_tasks(void) {
#if EN_TASK_TEST_ROUTINE == 1
    xTaskCreate(vTaskTestRoutine, TASK_TEST_ROUTINE_NAME, TASK_TEST_ROUTINE_STACK_SIZE, nullptr, TASK_TEST_ROUTINE_PRIORITY, &xTaskTestRoutineHandle);
    
    //! Set the core affinity mask for the task */
    vTaskCoreAffinitySet(xTaskTestRoutineHandle, TASK_TEST_ROUTINE_CORE);

    if (xTaskTestRoutineHandle == NULL) {
        //! Error creating the startup task
    }
//! EN_TASK_TEST_ROUTINE    
#endif


#if EN_TASK_HEARTBEAT == 1
    xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, nullptr, TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);
    
    //! Set the core affinity mask for the task */
    vTaskCoreAffinitySet(xTaskHeartbeatHandle, TASK_HEARTBEAT_CORE);

    if (xTaskHeartbeatHandle == NULL) {
        //! Error creating the startup task
    }
//! EN_TASK_HEARTBEAT    
#endif
}

void create_event_groups(void)
{
    task_startup_status = xEventGroupCreate();

    if (task_startup_status == NULL)
    {
        //! Error creating the startup status event group
    }
}
