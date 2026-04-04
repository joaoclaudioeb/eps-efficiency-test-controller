/**
@file pwm_module.h
@brief TODO.
@details TODO.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#ifndef PWM_MODULE_H_
#define PWM_MODULE_H_

#include <stdint.h>

#include "hardware/pwm.h"

typedef enum {
    PWM_P00 = 0,  PWM_P01 = 1,  PWM_P02 = 2,  PWM_P03 = 3,
    PWM_P04 = 4,  PWM_P05 = 5,  PWM_P06 = 6,  PWM_P07 = 7,
    PWM_P08 = 8,  PWM_P09 = 9,  PWM_P10 = 10, PWM_P11 = 11,
    PWM_P12 = 12, PWM_P13 = 13, PWM_P14 = 14, PWM_P15 = 15,
    PWM_P16 = 16, PWM_P17 = 17, PWM_P18 = 18, PWM_P19 = 19,
    PWM_P20 = 20, PWM_P21 = 21, PWM_P22 = 22, PWM_P23 = 23,
    PWM_P24 = 24, PWM_P25 = 25, PWM_P26 = 26, PWM_P27 = 27,
    PWM_P28 = 28, PWM_P29 = 29
} pwm_id_t;

typedef enum {
    PWM_DISABLE = 0,
    PWM_ENABLE  = 1
} pwm_state_t;

typedef struct {
    pwm_id_t pin;
    int duty_cycle;
    int frequency;
} pwm_config_t;

void pwm_config_pin(pwm_config_t config);
void pwm_enable_pin(pwm_config_t config, pwm_state_t state);
void pwm_set_duty_cycle(pwm_config_t config);
int pwm_get_slice_for_pin(gpio_id_t pin);

#endif /* PWM_MODULE_H_ */
