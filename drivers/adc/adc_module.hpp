#ifndef ADC_MODULE_HPP_
#define ADC_MODULE_HPP_

#include "adc_core.hpp"
#include <stdint.h>
#include <stddef.h>

namespace drivers {
    
    enum AdcPort {
        ADC_PORT_0 = 0,
        ADC_PORT_1,
        ADC_PORT_2,
        ADC_PORT_3
    };
    
    struct AdcConfig {
        AdcPort port;
    };

    class AdcController {
    public:
        AdcConfig config;
        AdcController(AdcConfig cfg) : config{cfg} {
            adapter_ = reinterpret_cast<adc_adapter*>(match_adc_adapter(config.port));
            
            this->init();
        }
        int init(void);
        int read(void);
    private:
        adc_adapter *adapter_;
    };
} // namespace drivers
#endif /* UART_MODULE_H_ */
