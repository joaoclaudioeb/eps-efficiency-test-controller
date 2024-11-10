#include <cstring>

#include <stddef.h>
#include <stdint.h>

#include "adc_core.hpp"
#include "adc_module.hpp"

int drivers::AdcController::init(void)
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->init)
        return -1;

    adapter_->init(adapter_->priv_data);

    return 0;
}

int drivers::AdcController::read(void)
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->read)
        return -1;

    return adapter_->read(adapter_->priv_data);
}
