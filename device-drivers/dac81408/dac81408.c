/**
@file dac81408.c
@brief Source file for controlling TI's 16-bit DAC, the DAC81408.
@details This code was based on FelipeJuliano24's one (https://github.com/FelipeJuliano24/DAC81408_lib/blob/main/dac81408.c).
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 04/04/2026
*/

#include "dac81408.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
 
#ifndef DAC81408_SPI_INSTANCE
#define DAC81408_SPI_INSTANCE spi0
#endif

#ifndef DAC81408_SPI_BAUDRATE
#define DAC81408_SPI_BAUDRATE 8000000
#endif

static inline void _dac81408_tcsh_delay(void) {
    sleep_us(1);  // minimum tCSH from datasheet
}

void dac81408_init(dac81408_t *dev,
                   uint8_t ldac_pin,
                   uint8_t reset_pin,
                   uint8_t cs_pin,
                   uint8_t miso_pin,
                   uint8_t sclk_pin,
                   uint8_t mosi_pin)
{    
    if (cs_pin != 0) {
        dev->cs_pin = cs_pin;
        gpio_init(cs_pin);
        gpio_set_dir(cs_pin, GPIO_OUT);
        gpio_put(dev->cs_pin, 1);
    }
    if (miso_pin != 0) {
        dev->miso_pin = miso_pin;
    }
    if (sclk_pin != 0) {
        dev->sclk_pin = sclk_pin;
    }
    if (mosi_pin != 0) {
        dev->mosi_pin = mosi_pin;
    }
    if (reset_pin != 0) {
        dev->reset_pin = reset_pin;
        gpio_init(reset_pin);
        gpio_set_dir(reset_pin, GPIO_OUT);
        gpio_put(dev->reset_pin, 1);
    }
    if (ldac_pin != 0) {
        dev->ldac_pin = ldac_pin;
        gpio_init(ldac_pin);
        gpio_set_dir(ldac_pin, GPIO_OUT);
        gpio_put(dev->ldac_pin, 1);
    }
}

int dac81408_initialize(dac81408_t *dev)
{
    // SPI config.
    spi_init(DAC81408_SPI_INSTANCE, DAC81408_SPI_BAUDRATE);
    gpio_set_function(dev->mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(dev->miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(dev->sclk_pin, GPIO_FUNC_SPI);

    // Force DAC81408 to reset...
    if(dev->reset_pin != 0) {
        gpio_put(dev->reset_pin, 0);
        sleep_ms(1); 
        gpio_put(dev->reset_pin, 1);; 
        sleep_ms(1);
    }

    // Set SPICONFIG: DEV_PWDN=0, FSDO=1
    uint16_t def = DAC81408_TEMPALM_EN(1) |
                   DAC81408_DACBUSY_EN(0) |
                   DAC81408_CRCALM_EN(1)  |
                   DAC81408_DEV_PWDWN(0)  |
                   DAC81408_CRC_EN(0)     |
                   DAC81408_SDO_EN(1)     |
                   DAC81408_FSDO(1)       |
                   (0x2 << 6);
    dac81408_write_register(dev, DAC81408_REG_SPICONFIG, def);

    // Reading SPICONFIG back
    uint16_t readback = dac81408_read_register(dev, DAC81408_REG_SPICONFIG);
    
    return (readback == def) ? 0 : -1;
}

void dac81408_write_register(dac81408_t *dev, uint8_t reg, uint16_t wdata)
{
    uint8_t lsb = ((uint16_t)wdata >> 0) & 0xFF;
    uint8_t msb = ((uint16_t)wdata >> 8) & 0xFF;
    
    spi_set_format(DAC81408_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_put(dev->cs_pin, 0);

    uint8_t cmd = reg;
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);

    cmd = msb;
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);

    cmd = lsb;
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);
    
    _dac81408_tcsh_delay();

    gpio_put(dev->cs_pin, 1);
}

uint16_t dac81408_read_register(dac81408_t *dev, uint8_t reg)
{
    uint8_t buf[3];

    spi_set_format(DAC81408_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_put(dev->cs_pin, 0);

    uint8_t cmd = (DAC81408_RREG | reg);
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);

    cmd = 0x00;
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);

    cmd = 0x00;
    spi_write_blocking(DAC81408_SPI_INSTANCE, &cmd, 1);

    _dac81408_tcsh_delay();

    gpio_put(dev->cs_pin, 1);

    gpio_put(dev->cs_pin, 0);
    
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[0], 1);
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[1], 1);
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[2], 1);

    _dac81408_tcsh_delay();

    gpio_put(dev->cs_pin, 1);

    return ((buf[1] << 8) | buf[2]);
}

void dac81408_set_ch_enabled(dac81408_t *dev, int ch, bool state)
{
    if (ch < 0 || ch > 7) return;
    
    uint16_t res = dac81408_read_register(dev, DAC81408_REG_DACPWDWN);
    uint16_t mask = 1 << (ch + 4);

    if (state) {
        dac81408_write_register(dev, DAC81408_REG_DACPWDWN, res & ~mask);  // Power UP (bit=0)
    } else {
        dac81408_write_register(dev, DAC81408_REG_DACPWDWN, res | mask);   // Power DOWN (bit=1)
    }
}

bool dac81408_get_ch_enabled(dac81408_t *dev, int ch)
{
    if (ch < 0 || ch > 7) return false;
    
    uint16_t res = dac81408_read_register(dev, DAC81408_REG_DACPWDWN);
    return !((res >> (ch + 4)) & 1);
}

void dac81408_set_int_reference(dac81408_t *dev, dac81408_ref_t state)
{
    uint16_t def = 0x0000;
    
    if (state) {
        dac81408_write_register(dev, DAC81408_REG_GENCONFIG, def);
    } else {
        def = 0x4000;
        dac81408_write_register(dev, DAC81408_REG_GENCONFIG, def);
    }
}

int dac81408_get_int_reference(dac81408_t *dev)
{
    int out = -1;
    
    uint16_t res = dac81408_read_register(dev, DAC81408_REG_GENCONFIG);

    if(res == 0x4000) out = 0;
    else if(res == 0) out = 1;

    return out;
}

void dac81408_set_range(dac81408_t *dev, int ch, dac81408_range_t range) {
    if (ch < 0 || ch > 7) return;
    
    // Select the correct register based on the channel
    uint8_t range_reg = (ch < 4) ? DAC81408_REG_DACRANGE0 : DAC81408_REG_DACRANGE1;
    uint8_t local_ch = (ch < 4) ? ch : ch - 4;  // Index 0-3 within the register
    
    // Read-modify-write via SPI (no cache)
    uint16_t current = dac81408_read_register(dev, range_reg);
    uint16_t mask = 0xF << (4 * local_ch);
    uint16_t write = (current & ~mask) | ((range << (4 * local_ch)) & mask);
    
    dac81408_write_register(dev, range_reg, write);
}

int dac81408_get_range(dac81408_t *dev, int ch) {
    if (ch < 0 || ch > 7) return -1;
    
    uint8_t range_reg = (ch < 4) ? DAC81408_REG_DACRANGE0 : DAC81408_REG_DACRANGE1;
    uint8_t local_ch = (ch < 4) ? ch : ch - 4;
    
    uint16_t val = dac81408_read_register(dev, range_reg);
    return (val >> (4 * local_ch)) & 0xF;
}

void dac81408_set_out(dac81408_t *dev, int ch, uint16_t val)
{
    if (ch < 0 || ch > 7) {
        // Handle the error (e.g., return or throw an exception)
        return;
    }

    uint8_t address = 0x14 + ch;

    dac81408_write_register(dev, address, val);
}

uint16_t dac81408_get_out(dac81408_t *dev, uint8_t reg)
{
    uint16_t val = dac81408_read_register(dev, reg);

    return val;
}

void dac81408_set_sync(dac81408_t *dev, int ch, dac81408_sync_t mode)
{
    uint16_t read = dac81408_read_register(dev, DAC81408_REG_SYNCCONFIG);

    if(mode==DAC81408_SYNC_LDAC) read |= 1UL << ch;
    else read &= ~(1UL << ch);

    dac81408_write_register(dev, DAC81408_REG_SYNCCONFIG, read);
}
