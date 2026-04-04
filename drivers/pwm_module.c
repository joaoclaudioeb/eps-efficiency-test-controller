/**
@file pwm_module.c
@brief TODO.
@details TODO.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#include "pwm_module.h"

void pwm_config_pin(pwm_config_t config)
{
    gpio_set_function((uint)config.pin, GPIO_FUNC_PWM);
}

void pwm_enable_pin(pwm_config_t config, pwm_state_t state)
{
    uint slice_num = pwm_gpio_to_slice_num((uint)config.pin);
    
    pwm_set_enabled(slice_num, (bool)state);
}

void pwm_set_duty_cycle(pwm_config_t config)
{
    uint slice_num = pwm_gpio_to_slice_num((uint)config.pin);
    uint channel = pwm_gpio_to_channel((uint)config.pin);
    
    const uint32_t SYS_CLK_HZ = 125000000;
    
    uint16_t wrap = (uint16_t)((SYS_CLK_HZ / (uint32_t)config.frequency) - 1);
    
    uint16_t level = (uint16_t)(((uint32_t)wrap * (uint32_t)config.duty_cycle) / 100U);
    
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, channel, level);
}

int pwm_get_slice_for_pin(gpio_id_t pin)
{
    return (int)pwm_gpio_to_slice_num((uint)pin);
}
