/**
@file gpio_module.h
@brief TODO.
@details TODO.
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#ifndef GPIO_MODULE_H_
#define GPIO_MODULE_H_

#include <stdint.h>
#include <stdbool.h>

#include "hardware/gpio.h"

typedef enum {
    GPIO_P00 = 0,  GPIO_P01 = 1,  GPIO_P02 = 2,  GPIO_P03 = 3,
    GPIO_P04 = 4,  GPIO_P05 = 5,  GPIO_P06 = 6,  GPIO_P07 = 7,
    GPIO_P08 = 8,  GPIO_P09 = 9,  GPIO_P10 = 10, GPIO_P11 = 11,
    GPIO_P12 = 12, GPIO_P13 = 13, GPIO_P14 = 14, GPIO_P15 = 15,
    GPIO_P16 = 16, GPIO_P17 = 17, GPIO_P18 = 18, GPIO_P19 = 19,
    GPIO_P20 = 20, GPIO_P21 = 21, GPIO_P22 = 22, GPIO_P23 = 23,
    GPIO_P24 = 24, GPIO_P25 = 25, GPIO_P26 = 26, GPIO_P27 = 27,
    GPIO_P28 = 28, GPIO_P29 = 29
} gpio_id_t;

typedef enum {
    GPIO_FUNCT_XIP   = 0,
    GPIO_FUNCT_SPI   = 1,
    GPIO_FUNCT_UART  = 2,
    GPIO_FUNCT_I2C   = 3,
    GPIO_FUNCT_PWM   = 4,
    GPIO_FUNCT_SIO   = 5,
    GPIO_FUNCT_PIO0  = 6,
    GPIO_FUNCT_PIO1  = 7,
    GPIO_FUNCT_GPCK  = 8,
    GPIO_FUNCT_USB   = 9,
    GPIO_FUNCT_NONE  = 0x1F
} gpio_func_t;

typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1
} gpio_dir_t;

typedef enum {
    GPIO_LOW  = 0,
    GPIO_HIGH = 1
} gpio_state_t;

void gpio_init_pin(gpio_id_t pin);
void gpio_set_pin_function(gpio_id_t pin, gpio_func_t function);
void gpio_set_pin_direction(gpio_id_t pin, gpio_dir_t dir);
void gpio_set_pin_state(gpio_id_t pin, gpio_state_t state);
int gpio_get_pin_state(gpio_id_t pin);
void gpio_toggle_pin(gpio_id_t pin);

#endif /* GPIO_MODULE_H_ */
