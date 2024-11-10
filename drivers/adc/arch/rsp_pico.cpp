#include "drivers/adc/adc_core.hpp"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h" 
#include <stdint.h>
#include <cstdint>           
#include <cstdio>            // Adicionado para sprintf
#include <string>            // Adicionado para std::string       


#define ADC_GENERIC_INPUT_CHN_0    0     
#define ADC_GENERIC_INPUT_CHN_1    1
#define ADC_GENERIC_INPUT_CHN_2    2
#define ADC_TEMP_SENSOR_INPUT      4

#define ADC_PIN_CHN_0              26     
#define ADC_PIN_CHN_1              27
#define ADC_PIN_CHN_2              28
#define ADC_PIN_TEMP_SENSOR        29

struct adc_dev {
    unsigned int port;
    unsigned int pin;
};

int rspAdcInit(void* priv_data)
{
    adc_dev *dev = reinterpret_cast<adc_dev*>(priv_data);
    char buffer[20];
    adc_init();

    switch (dev->pin) {
    case ADC_PIN_TEMP_SENSOR:
        uart_puts(uart0, "certo");
        adc_set_temp_sensor_enabled (true);
        break;
    default:
        uart_puts(uart0, "errado");
        adc_gpio_init(dev->pin);
        break;
    }
    sprintf(buffer, "%d", dev->pin); // Corrigido: Removido o ponto e vírgula extra
    std::string str(buffer);
    uart_puts(uart0, str.c_str());
    return 0;
}

int rspAdcRead(void* priv_data)
{
    char buffer[20];
    int value = 0;
    adc_dev *dev = reinterpret_cast<adc_dev*>(priv_data);

    adc_select_input(dev->port);

    sprintf(buffer, "%d", dev->port); // Corrigido: Removido o ponto e vírgula extra
    std::string str(buffer);
    uart_puts(uart0, str.c_str());
    value = adc_read();

    sprintf(buffer, "%d", value); // Corrigido: Removido o ponto e vírgula extra
    str = std::string(buffer);
    uart_puts(uart0, str.c_str());
    
    return value;
}

static adc_dev adc_0 {
    .port = ADC_GENERIC_INPUT_CHN_0,
    .pin = ADC_PIN_CHN_0,
};

static adc_dev adc_1 {
    .port = ADC_GENERIC_INPUT_CHN_1,
    .pin = ADC_PIN_CHN_1,    
};

static adc_dev adc_2 {
    .port = ADC_GENERIC_INPUT_CHN_2,
    .pin = ADC_PIN_CHN_2,
};

static adc_dev adc_3 {
    .port = ADC_TEMP_SENSOR_INPUT,
    .pin = ADC_PIN_TEMP_SENSOR,
};

static adc_adapter adc_adap_0 = {
    .priv_data = &adc_0,
    .init = rspAdcInit,
    .read = rspAdcRead,
};

static adc_adapter adc_adap_1 = {
    .priv_data = &adc_1,
    .init = rspAdcInit,
    .read = rspAdcRead,
};

static adc_adapter adc_adap_2 = {
    .priv_data = &adc_2,
    .init = rspAdcInit,
    .read = rspAdcRead,
};

static adc_adapter adc_adap_3 = {
    .priv_data = &adc_3,
    .init = rspAdcInit,
    .read = rspAdcRead,
};

void *match_adc_adapter(unsigned int port)
{
    switch (port) {
    case 0:
        return &adc_adap_0;
    case 1:
        return &adc_adap_1;
    case 2:
        return &adc_adap_2;
    case 3:
        return &adc_adap_3;
    default:
        return nullptr;
    }
}
