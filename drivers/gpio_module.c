/**
@file gpio_module.c
@brief TODO.
@details TODO.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#include "gpio_module.h"

void gpio_init_pin(gpio_id_t pin)
{
    gpio_init((uint)pin);
}

void gpio_set_pin_function(gpio_id_t pin, gpio_func_t function)
{
    gpio_set_function((uint)pin, (gpio_function_t)function);
}

void gpio_set_pin_direction(gpio_id_t pin, gpio_dir_t dir)
{
    gpio_set_dir((uint)pin, (bool)dir);
}

void gpio_set_pin_state(gpio_id_t pin, gpio_state_t state)
{
    gpio_put((uint)pin, (bool)state);
}

int gpio_get_pin_state(gpio_id_t pin)
{
    return (int)gpio_get((uint)pin);
}

void gpio_toggle_pin(gpio_id_t pin)
{
    gpio_xor_mask(1U << (uint)pin);
}
