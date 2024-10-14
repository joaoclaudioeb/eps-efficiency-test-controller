#include "drivers/gpio/gpio_core.hpp"
#include "hardware/gpio.h"
#include <stdint.h>
#include <hardware/gpio.h>

struct gpio_dev {
    unsigned int pin;
    gpio_function func;
    bool dir;
    bool state;
};

int rspInitPin(void* priv_data, unsigned int pin) {
    
    gpio_dev *dev = reinterpret_cast<gpio_dev*>(priv_data);
    dev->pin = pin;

    gpio_init(dev->pin);

    return 0;
}

int rspSetFunc(void* priv_data, unsigned int pin, gpio_function func) {
    
    gpio_dev *dev = reinterpret_cast<gpio_dev*>(priv_data);
    dev->pin = pin;
    dev->func = func;

    gpio_set_function(dev->pin, dev->func);

    return 0;
}

int rspSetDir(void* priv_data, unsigned int pin, bool dir) {
    
    gpio_dev *dev = reinterpret_cast<gpio_dev*>(priv_data);
    dev->pin = pin;
    dev->dir = dir;

    gpio_set_dir(dev->pin, dev->dir);

    return 0;
}

int rspSetState(void* priv_data, unsigned int pin, bool state) {
    
    gpio_dev *dev = reinterpret_cast<gpio_dev*>(priv_data);
    dev->pin = pin;
    dev->state = state;

    gpio_put(dev->pin, dev->dir);

    return 0;
}

bool rspGetState(void* priv_data, unsigned int pin) {
    
    gpio_dev *dev = reinterpret_cast<gpio_dev*>(priv_data);
    dev->pin = pin;

    return gpio_get_out_level(dev->pin);
}

static gpio_dev gpio {
    .pin = 0U,
    .func = GPIO_FUNC_NULL,
    .dir = 0U,
    .state = 0U,
};

static gpio_adapter gpio_adap = {
    .priv_data = &gpio,
    .init = rspInitPin,
    .func = rspSetFunc,
    .dir = rspSetDir,
    .set = rspSetState,
    .get = rspGetState,
};

void *match_adapter(void)
{
    return &gpio_adap;
}
