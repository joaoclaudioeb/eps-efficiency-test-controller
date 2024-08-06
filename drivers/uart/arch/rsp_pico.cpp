#include "drivers/uart/uart_core.h"
#include "hardware/gpio.h"
#include <stdint.h>
#include <hardware/uart.h>

#define GPIO_UART0_TX_PIN 0
#define GPIO_UART0_RX_PIN 1

struct uart_dev {
    uart_inst_t *port;
    unsigned int flags;
    unsigned int baud;
};

int rsp_uart_init(void* priv_data, unsigned int flags)
{
    uart_dev *dev = reinterpret_cast<uart_dev*>(priv_data);

    dev->flags = flags;

    dev->baud = 115200;

    uint baud = uart_init(dev->port, dev->baud);

    uart_set_hw_flow(dev->port, false, false);

    gpio_set_function(GPIO_UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPIO_UART0_RX_PIN, GPIO_FUNC_UART);

    uart_set_format(dev->port, 8, 1, UART_PARITY_NONE);

    return 0;
}

int rsp_uart_write(void* priv_data, const uint8_t *data, const uint32_t size)
{
    uart_dev *dev = reinterpret_cast<uart_dev*>(priv_data);

    uart_write_blocking(dev->port, data, size);

    return 0;
}

int rsp_uart_read(void* priv_data, uint8_t *data, const uint32_t size)
{
    uart_dev *dev = reinterpret_cast<uart_dev*>(priv_data);

    uart_read_blocking(dev->port, data, size);

    return 0;
}

static uart_dev uart_1 {
    .port = uart1,
    .flags = 0U,
    .baud = 0U,
};

static uart_dev uart_0 {
    .port = uart0,
    .flags = 0U,
    .baud = 0U,
};

static uart_adapter uart_adap_0 = {
    .priv_data = &uart_0,
    .init = rsp_uart_init,
    .write = rsp_uart_write,
    .read = rsp_uart_read,
    .pool = nullptr,
};

static uart_adapter uart_adap_1 = {
    .priv_data = &uart_1,
    .init = rsp_uart_init,
    .write = rsp_uart_write,
    .read = rsp_uart_read,
    .pool = nullptr,
};

void *match_adapter(unsigned int port, unsigned int flags)
{
    switch (port) {
        case 0:
            return &uart_adap_0;
        case 1:
            return &uart_adap_1;
        default:
            return nullptr;
    }
}
