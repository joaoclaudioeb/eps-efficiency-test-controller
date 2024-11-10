#ifndef ADC_CORE_HPP_
#define ADC_CORE_HPP_

#include <stdint.h>

struct adc_adapter {
    void *priv_data;
    int (*init)(void* priv_data);
    int (*read)(void* priv_data);
};

void *match_adc_adapter(unsigned int port);

#endif
