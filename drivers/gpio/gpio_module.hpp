//!
//! @file gpio_module.hpp
//!
//! @brief TODO.
//!
//! @details TODO. 
//!
//! @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
//! @version 0.0.0
//! @date 29/07/2024
//!


#ifndef GPIO_MODULE_HPP_
#define GPIO_MODULE_HPP_

//! Raspberry-related headers
#include "hardware/gpio.h"
#include "gpio_core.hpp"

namespace drivers {
    
//! Enumerations
enum class gpio_id {
    P00 = 0 ,  P01 = 1 ,  P02 = 2 ,  P03 = 3 ,  P04 = 4 ,  P05 = 5 ,  P06 = 6 ,
    P07 = 7 ,  P08 = 8 ,  P09 = 9 ,  P10 = 10 , P11 = 11 , P12 = 12 , P13 = 13 ,
    P14 = 14 , P15 = 15 , P16 = 16 , P17 = 17 , P18 = 18 , P19 = 19 , P20 = 20 ,
    P21 = 21 , P22 = 22 , P23 = 23 , P24 = 24 , P25 = 25 , P26 = 26 , P27 = 27 ,
    P28 = 28
};

enum class gpio_func {
    XIP = 0 , SPI = 1 , UART = 2 , I2C = 3 ,
    PWM = 4 , SIO = 5 , PIO0 = 6 , PIO1 = 7 ,
    GPCK = 8 , USB = 9 , NONE = 0x1F
};

enum class gpio_dir {
    INPUT = 0,
    OUTPUT = 1
};

enum class gpio_state {
    HIGH = 1,
    LOW = 0
};

    
//!
//! @class GPIO
//!
//! @brief TODO.
//!
//! @details TODO.
//!
class GpioController {
public:
    GpioController();
    ~GpioController();
          
    int init(gpio_id pin);
    int func(gpio_id pin, gpio_func func);
    //! void setMaskedFunction(gpio_id pin, gpio_function function);
    int dir(gpio_id pin, gpio_dir dir);
    //! void setMaskedDirection(gpio_id pin, gpio_dir dir);
    int set(gpio_id pin, gpio_state state);
    //! void setMaskedState(gpio_id pin, gpio_state state);
    bool get(gpio_id pin);
    int toggle(gpio_id pin);
private:
    gpio_adapter *adapter_;
};
}
//! GPIO_MODULE_HPP_
#endif
