/**
 * @file gpio_module.cpp
 *
 * @brief TODO.
 *
 * @details TODO. 
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 * @date 29/07/2024
 */

/* Abstraction-related headers */
#include "gpio_module.h"

GPIO::GPIO() {}
GPIO::~GPIO() {}

void GPIO::initPin(gpio_id pin) {
    gpio_init(static_cast<unsigned int>(pin));
}
void GPIO::setFunction(gpio_id pin, gpio_function function) {
    gpio_set_function(static_cast<unsigned int>(pin), function);
}

void GPIO::setDirection(gpio_id pin, gpio_dir dir) {
    gpio_set_dir(static_cast<unsigned int>(pin), static_cast<bool>(dir));
}

void GPIO::setState(gpio_id pin, gpio_state state) {
    gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(state));
}

bool GPIO::getState(gpio_id pin) {
    return gpio_get_out_level(static_cast<unsigned int>(pin));
}

void GPIO::togglePin(gpio_id pin) {
    bool state = getState(pin);
    if(state == true)
        gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(gpio_state::LOW));
    else
        gpio_put(static_cast<unsigned int>(pin), static_cast<bool>(gpio_state::HIGH));
}
