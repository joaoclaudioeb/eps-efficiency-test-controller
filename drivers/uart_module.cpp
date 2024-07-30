/**
 * @file uart_module.cpp
 *
 * @brief This file contains the source code defining the class for the UART module.
 *
 * @author João Cláudio Elsen Barcellos <joao.barcellos@posgrad.ufsc.br>
 * @version 0.0.0
 *
 * @date 29/07/2024
 */

#include "uart_module.h"

Serial::Serial(uart_inst_t *uart, uart_parity_t parity, uint baud_rate, uint data_bits, uint stop_bits, uint tx_pin, uint rx_pin) {
    uart_init(uart, baud_rate); //

    gpio_set_function(tx_pin, GPIO_FUNC_UART); //
    gpio_set_function(rx_pin, GPIO_FUNC_UART); //

    uart_set_hw_flow(uart, false, false); //

    uart_set_format(uart, data_bits, stop_bits, parity); //
}

Serial::~Serial() {}

void Serial::sendPackage(string message) {
    uart_puts(DEFAULT_UART_CHANNEL, message.c_str());
}

string Serial::receivePackage() {
    string message;
    bool received = false;
    
    while (!received) {
        if (uart_is_readable(DEFAULT_UART_CHANNEL)) {
            uint8_t data = uart_getc(DEFAULT_UART_CHANNEL);
            if (data == '\n') { // 
                received = true;
            }
            message += data;
        }

        sleep_ms(100); //
    }

    return message;
}
