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

void GPIO::initMultiPins(const std::vector<gpio_id>& pins) {
    uint32_t mask = 0;
    
    for (const auto& pin : pins) {
        mask |= (1 << static_cast<unsigned int>(pin)); // Cria a máscara a partir do vetor de pinos
    }
    
    gpio_init_mask(mask); // Configura os pinos como saída
}

void GPIO::setMultiOut(const std::vector<gpio_id>& pins) {
    uint32_t mask = 0;
    
    for (const auto& pin : pins) {
        mask |= (1 << static_cast<unsigned int>(pin)); // Cria a máscara a partir do vetor de pinos
    }
    
    gpio_set_dir_out_masked(mask); // Configura os pinos como saída
}

void GPIO::setMultiState(const std::vector<gpio_id>& pins, uint32_t value) {
    uint32_t mask = 0;
    uint32_t newValue = 0;
    uint32_t size = pins.size() - 1;

    for (const auto& pin : pins) {
        mask |= (1 << static_cast<unsigned int>(pin)); // Cria a máscara a partir do vetor de pinos
    }

    for (size_t i = 0; i <= size; ++i) {
        // Verifica o bit correspondente em 'value'
        if (value & (1 << i)) {
            // Liga o pino se o bit for 1
            newValue |= (1 << static_cast<unsigned int>(pins[size - i])); 
        } else {
            // Desliga o pino se o bit for 0
            newValue &= ~(1 << static_cast<unsigned int>(pins[size - i])); // Garantir que o pino esteja desligado
        }
    }

    gpio_put_masked(mask, newValue); // Altera o estado dos pinos
}
