/**
@file ads1256.h
@brief Header file for controlling TI's 24-bit ADC, the ADS1256.
@details This code was based on CuriousScientist0's one (https://github.com/CuriousScientist0/ADS1256/blob/main/src/ADS1256.h and https://www.youtube.com/watch?v=GBWJdyjRIdM&t=262s).
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#ifndef ADS1256_H_
#define ADS1256_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define ADS1256_REG_ADDR_STATUS   0x00
#define ADS1256_REG_ADDR_MUX      0x01
#define ADS1256_REG_ADDR_ADCON    0x02
#define ADS1256_REG_ADDR_DRATE    0x03
#define ADS1256_REG_ADDR_IO       0x04
#define ADS1256_REG_ADDR_OFC0     0x05
#define ADS1256_REG_ADDR_OFC1     0x06
#define ADS1256_REG_ADDR_OFC2     0x07
#define ADS1256_REG_ADDR_FSC0     0x08
#define ADS1256_REG_ADDR_FSC1     0x09
#define ADS1256_REG_ADDR_FSC2     0x0A

#define ADS1256_CMD_WAKEUP        0xFF
#define ADS1256_CMD_RDATA         0x01
#define ADS1256_CMD_RDATAC        0x03
#define ADS1256_CMD_SDATAC        0x0F
#define ADS1256_CMD_RREG          0x10
#define ADS1256_CMD_WREG          0x50
#define ADS1256_CMD_SELFCAL       0xF0
#define ADS1256_CMD_SELFOCAL      0xF1
#define ADS1256_CMD_SELFGCAL      0xF2
#define ADS1256_CMD_SYSOCAL       0xF3
#define ADS1256_CMD_SYSGCAL       0xF4
#define ADS1256_CMD_SYNC          0xFC
#define ADS1256_CMD_STANDBY       0xFD
#define ADS1256_CMD_RESET         0xFE

typedef enum {
    ADS1256_BITORDER_MSB          = 0,
    ADS1256_BITORDER_LSB          = 1
} ads1256_bitorder_t;

typedef enum {
    ADS1256_ACAL_DISABLED         = 0,
    ADS1256_ACAL_ENABLED          = 1
} ads1256_acal_t;

typedef enum {
    ADS1256_BUFFER_DISABLED       = 0,
    ADS1256_BUFFER_ENABLED        = 1
} ads1256_buffer_t;

typedef enum {
    ADS1256_MUX_DIFF_0_1          = 0x01,
    ADS1256_MUX_DIFF_2_3          = 0x23,
    ADS1256_MUX_DIFF_4_5          = 0x45,
    ADS1256_MUX_DIFF_6_7          = 0x67,
    ADS1256_MUX_SING_0            = 0x0F,
    ADS1256_MUX_SING_1            = 0x1F,
    ADS1256_MUX_SING_2            = 0x2F,
    ADS1256_MUX_SING_3            = 0x3F,
    ADS1256_MUX_SING_4            = 0x4F,
    ADS1256_MUX_SING_5            = 0x5F,
    ADS1256_MUX_SING_6            = 0x6F,
    ADS1256_MUX_SING_7            = 0x7F,
} ads1256_mux_t;

typedef enum {
    ADS1256_PGA_1                 = 0x00,
    ADS1256_PGA_2                 = 0x01,
    ADS1256_PGA_4                 = 0x02,
    ADS1256_PGA_8                 = 0x03,
    ADS1256_PGA_16                = 0x04,
    ADS1256_PGA_32                = 0x05,
    ADS1256_PGA_64                = 0x06,
} ads1256_pga_t;

typedef enum {
    ADS1256_DRATE_30000SPS        = 0xF0,
    ADS1256_DRATE_15000SPS        = 0xE0,
    ADS1256_DRATE_7500SPS         = 0xD0,
    ADS1256_DRATE_3750SPS         = 0xC0,
    ADS1256_DRATE_2000SPS         = 0xB0,
    ADS1256_DRATE_1000SPS         = 0xA1,
    ADS1256_DRATE_500SPS          = 0x92,
    ADS1256_DRATE_100SPS          = 0x82,
    ADS1256_DRATE_60SPS           = 0x72,
    ADS1256_DRATE_50SPS           = 0x63,
    ADS1256_DRATE_30SPS           = 0x53,
    ADS1256_DRATE_25SPS           = 0x43,
    ADS1256_DRATE_15SPS           = 0x33,
    ADS1256_DRATE_10SPS           = 0x23,
    ADS1256_DRATE_5SPS            = 0x13,
    ADS1256_DRATE_2SPS            = 0x03,
} ads1256_drate_t;

struct ads1256 {
    // GPIOs and SPI pins
    uint8_t drdy_pin;
    uint8_t reset_pin;
    uint8_t sync_pin;
    uint8_t cs_pin;
    uint8_t miso_pin;
    uint8_t sclk_pin;
    uint8_t mosi_pin;
    
    // Voltage ref.
    float vref;
    
    // Regs. values (local cache)
    uint8_t drate_reg;
    uint8_t adcon_reg;
    uint8_t mux_reg;
    uint8_t status_reg;
    uint8_t gpio_reg;
    
    // Internal state
    bool is_acquisition_running;
    uint8_t cycle;
    
    // Output buffer (3 bytes = 24 bits)
    uint8_t output_buffer[3];
    int32_t output_value;
};

typedef struct ads1256 ads1256_t;

void ads1256_init(ads1256_t *dev,
                  uint8_t drdy_pin,
                  uint8_t reset_pin,
                  uint8_t sync_pin,
                  uint8_t cs_pin,
                  uint8_t miso_pin,
                  uint8_t sclk_pin,
                  uint8_t mosi_pin,
                  float vref);

void ads1256_initialize(ads1256_t *dev);
uint8_t ads1256_read_register(ads1256_t *dev, uint8_t reg);
void ads1256_write_register(ads1256_t *dev, uint8_t reg, uint8_t value);
void ads1256_set_drate(ads1256_t *dev, ads1256_drate_t drate);
void ads1256_set_pga(ads1256_t *dev, ads1256_pga_t pga);
void ads1256_set_mux(ads1256_t *dev, ads1256_mux_t mux);
void ads1256_set_byte_order(ads1256_t *dev, ads1256_bitorder_t order);
void ads1256_set_buffer(ads1256_t *dev, ads1256_buffer_t buff_en);
void ads1256_set_autocal(ads1256_t *dev, ads1256_acal_t acal);
void ads1256_set_gpio_direction(ads1256_t *dev, uint8_t dir0, uint8_t dir1, uint8_t dir2, uint8_t dir3);
void ads1256_set_clkout(ads1256_t *dev, uint8_t clkout);
void ads1256_set_sdcs(ads1256_t *dev, uint8_t sdcs);
void ads1256_write_gpio(ads1256_t *dev, uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3);
int8_t ads1256_read_gpio(ads1256_t *dev, uint8_t gpio_pin);
void ads1256_send_command(ads1256_t *dev, uint8_t cmd);
int32_t ads1256_read_single(ads1256_t *dev);
int32_t ads1256_read_continuous(ads1256_t *dev);
void ads1256_read_burst(ads1256_t *dev, int32_t *buffer, uint32_t samples);
int32_t ads1256_cycle_single(ads1256_t *dev);
int32_t ads1256_cycle_differential(ads1256_t *dev);
float ads1256_convert_to_voltage(ads1256_t *dev, int32_t raw);
void ads1256_stop_continuous(ads1256_t *dev);
void ads1256_wait_drdy(ads1256_t *dev);
void ads1256_end_communication(ads1256_t *dev);
void ads1256_start_communication(ads1256_t *dev);
void ads1256_wakeup(ads1256_t *dev);
void ads1256_sync(ads1256_t *dev);

// Optional
// void ads1256_test_acquisition_rate(ads1256_t *dev, ads1256_drate_t drate, uint32_t packet_size);

#endif /* ADS1256_H_ */
