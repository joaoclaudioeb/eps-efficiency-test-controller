#include "uart_core.h"
#include <stddef.h>
#include <stdint.h>
#include <cstring>
#include "uart_module.h"

int drivers::UartController::init(void)
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->init)
        return -1;

    adapter_->init(adapter_->priv_data, 0U);

    return 0;
}

int drivers::UartController::write(const uint8_t *data, const size_t size)
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->write)
        return -1;

    adapter_->write(adapter_->priv_data, data, size);

    return 0;
}

int drivers::UartController::read(uint8_t *data, const size_t size)
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->read)
        return -1;

    adapter_->read(adapter_->priv_data, data, size);

    return 0;
}

int drivers::UartController::puts(const char *str)
{
    size_t size = strlen(str);

    return this->write(reinterpret_cast<const uint8_t*>(str), size);
}

int drivers::UartController::flush()
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->flush)
        return -1;

    adapter_->flush(adapter_->priv_data);

    return 0;
}

int drivers::UartController::poll()
{
    if (!adapter_ || !adapter_->priv_data || !adapter_->poll)
        return -1;

    return adapter_->poll(adapter_->priv_data);
}
