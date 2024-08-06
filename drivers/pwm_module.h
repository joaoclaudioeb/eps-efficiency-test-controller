/**
 * @file pwm_module.h
 *
 * @brief TODO.
 *
 * @details TODO. 
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 * @date 29/07/2024
 */


#ifndef PWM_MODULE_H_
#define PWM_MODULE_H_

/* Internal headers */
#include "gpio_module.h"

/* Raspberry-related headers */
#include "hardware/pwm.h"

/* Enumerations */
enum class pwm_state {
    ENABLE = 1,
    DISABLE = 0
};

/* Structs */
typedef struct {
    gpio_id pin;
    int duty_cycle;
    int frequency;
} pwm_config_t;
    
/**
 * @class PWM
 *
 * @brief TODO.
 *
 * @details TODO.
 */
class PWM {
private:   
    int err;
public:
    PWM();        
    ~PWM();
          
    void configPwmPin(pwm_config_t config);

    void enablePwmPin(pwm_config_t config, pwm_state state);

    void setDutyCycle(pwm_config_t config);
};

#endif /* GPIO_MODULE_H_ */
