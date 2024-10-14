/**
 * @file pwm_module.cpp
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
#include "pwm_module.h"

PWM::PWM() {}
PWM::~PWM() {}

void PWM::configPwmPin(pwm_config_t config) {
    gpio_set_function(static_cast<unsigned int>(config.pin), static_cast<gpio_function>(drivers::gpio_func::PWM));
}

void PWM::enablePwmPin(pwm_config_t config, pwm_state state) {
    unsigned int slice = pwm_gpio_to_slice_num(static_cast<unsigned int>(config.pin)); 
    pwm_set_enabled(slice, static_cast<bool>(state));
}

void PWM::setDutyCycle(pwm_config_t config) {
    unsigned int slice = pwm_gpio_to_slice_num(static_cast<unsigned int>(config.pin)); 
    unsigned int channel = pwm_gpio_to_channel(static_cast<unsigned int>(config.pin));

    unsigned short int wrap = (125000000/config.frequency) -1;
    unsigned short int level = (wrap*config.duty_cycle)/100;

    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, channel, level);
}
