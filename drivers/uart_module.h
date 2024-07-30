/**
 * @file uart_module.h
 *
 * @brief This file contains the header declaring the class for the UART module.
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 *
 * @date 29/07/2024
 */

#ifndef UART_MODULE_H_
#define UART_MODULE_H_

/* Raspberry-related libraries*/
#include "hardware/uart.h"
#include "hardware/irq.h"

/* Other libraries*/
#include <string>

/* Macros for configuring the UART*/
#define UART0 uart0
#define UART1 uart1
#define DEFAULT_UART_CHANNEL uart0
#define DEFAULT_BAUD_RATE 115200
#define DEFAULT_DATA_BITS 8
#define DEFAULT_STOP_BITS 1
#define DEFAULT_PARITY UART_PARITY_NONE
#define DEFAULT_TX_PIN 0
#define DEFAULT_RX_PIN 1

/* Keywords */
typedef unsigned int uint;
typedef std::string string;


/**
 * @class Serial
 *
 * @brief A class representing the UART module.
 *
 * @details Provides functionalities for UART communication and configuration.
 */
class Serial {
private:
    int err;

public:
    Serial(uart_inst_t *uart, uart_parity_t parity, uint baud_rate, uint data_bits, uint stop_bits, uint tx_pin, uint rx_pin);
    ~Serial();

    /**
     * @brief Sends a package over UART.
     *
     * @param[in] message is the message to be sent.
     */
    void sendPackage(string message);
    /**
     * @brief Receives a package over UART.
     *
     * @return The received package as a string.
     */
    string receivePackage();
};

#endif /* UART_MODULE_H_ */
