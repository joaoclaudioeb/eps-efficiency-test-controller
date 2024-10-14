//!
//! @file gpio_core.hpp
//!
//! @brief TODO.
//!
//! @details TODO. 
//!
//! @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
//! @version 0.0.0
//! @date 29/07/2024
//!


#ifndef GPIO_CORE_HPP_
#define GPIO_CORE_HPP_

#include "hardware/gpio.h"
#include <stdint.h>

struct gpio_adapter {
    void *priv_data;
    int (*init)(void* priv_data, unsigned int pin);
    int (*func)(void* priv_data, unsigned int pin, gpio_function func);
    int (*dir)(void* priv_data, unsigned int pin, bool dir);
    int (*set)(void* priv_data, unsigned int pin, bool state);
    bool (*get)(void* priv_data, unsigned int pin);
};

void *match_adapter(void);

//! GPIO_CORE_HPP_
#endif
