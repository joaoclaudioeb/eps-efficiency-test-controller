#ifndef UART_MODULE_H_
#define UART_MODULE_H_

#include "uart_core.h"
#include <stdint.h>
#include <stddef.h>

namespace drivers {

enum UartParity {
  UART_PARITY_NONE,
  UART_PARITY_ODD,
  UART_PARITY_EVEN,
};
enum UartBaudrate {
  UART_BD_115200,
  UART_BD_9600,
};
enum UartDataBits {
  UART_DB_8,
  UART_DB_7,
  UART_DB_6,
  UART_DB_5,
};

enum UartStopBits {
  UART_SB_0,
  UART_SB_1,
  UART_SB_2,
};

enum UartPort {
    UART_0 = 0,
    UART_1,
    UART_2,
};

struct UartConfig {
  UartBaudrate baud;
  UartDataBits db;
  UartStopBits sb;
  UartParity par;
  UartPort port;
};

class UartController {
public:
  UartConfig config;
  UartController(UartConfig cfg) : config{cfg} 
  {
      adapter_ = reinterpret_cast<uart_adapter*>(match_adapter(config.port, 0U));

      this->init();
  }
  int init(void);
  int write(const uint8_t *data, const size_t size);
  int read(uint8_t *data, const size_t size);
  int puts(const char *str);
  int poll();
  int flush();

private:
  unsigned int parse_config_to_flags(void);
  uart_adapter *adapter_;
};

} // namespace drivers
#endif /* UART_MODULE_H_ */
