//!
//! @file basic_test_routine.cpp
//!
//! @brief This file contains the basic test routine task implementation.
//!
//! @author Ramon de Araujo Borba <ramonborba97@gmail.com>
//! @version 0.0.0
//!
//! @date 06/08/2024
//!


//! C++ standard library
#include <cstdint>           
#include <cstdio>            // Adicionado para sprintf
#include <string>            // Adicionado para std::string       

//! External Libraries
//#include "hardware/uart.h"    //!< Raspberry Pi Pico SDK
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "projdefs.h"         //!< FreeRTOS

//! Project-specific Drivers
#include "drivers/gpio_module.h"
#include "drivers/adc/adc_module.hpp"
#include "drivers/uart/uart_module.h"

//! Project-specific Headers
#include "re2ps_read_routine.hpp"

TaskHandle_t xTaskRe2psReadRoutineHandle;

void vTaskRe2psReadRoutine(void *pvParameters) {

    char buffer[20];
    int adcValue = 0;
    
    drivers::UartConfig uart_cfg {
        .port = drivers::UartPort::UART_0,
    };
    drivers::UartController uart{uart_cfg};

    drivers::AdcConfig adc_cfg {
        .port = drivers::AdcPort::ADC_PORT_3,
    };
    drivers::AdcController adc{adc_cfg};
    
    //! Configure GPIO
    GPIO gpio;

    std::vector<gpio_id> mux_pins = {TOGGLE_MUX_CHN, MUX_BIT_0, MUX_BIT_1, MUX_BIT_2};

    gpio.initMultiPins(mux_pins);
    gpio.setMultiOut(mux_pins);
    gpio.setMultiState(mux_pins,0b0000);
    
    while (true) {
        for(int i = 0; i < 16; ++i) {
            gpio.setMultiState(mux_pins,(0b0000 + i));
            vTaskDelay(TASK_RE2PS_READ_ROUTINE_PERIOD_MS / 2);
        }  
        vTaskDelay(TASK_RE2PS_READ_ROUTINE_PERIOD_MS * 2);

        uart.puts("Ola, to vivo! \n");

         adcValue = adc.read(); 

         sprintf(buffer, "%d", adcValue); // Corrigido: Removido o ponto e vírgula extra
         std::string str(buffer);
         uart.puts(str.c_str()); // Use c_str() para obter a representação C da string
         uart.puts("\n"); // Use aspas para enviar uma nova linha
    }
};
