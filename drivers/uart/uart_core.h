#ifndef UART_CORE_H_
#define UART_CORE_H_

#include <stdint.h>

struct uart_adapter {
    void *priv_data;
    int (*init)(void* priv_data, unsigned int flags);
    int (*write)(void* priv_data, const uint8_t *data, const uint32_t size);
    int (*read)(void* priv_data, uint8_t *data, const uint32_t size);
    int (*poll)(void* priv_data);
    int (*flush)(void* priv_data);
};

void *match_adapter(unsigned int port, unsigned int flags);

#endif 

