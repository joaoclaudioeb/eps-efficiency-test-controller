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

static inline void _dac81408_tldac_delay(void) {
    sleep_us(1);  // minimum tLDAC from datasheet
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

    dev->dacrange0_cache = 0x0000;
    dev->dacrange1_cache = 0x0000;
    dev->genconfig_cache = 0x7F00;  // REF-PWDWN=1, RESERVED bits
    dev->syncconfig_cache = 0x0000;
    dev->dacpwdwn_cache = 0x0FF0;   // All channels powered down
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
        gpio_put(dev->reset_pin, 1); 
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

    gpio_put(dev->cs_pin, 1);

    _dac81408_tcsh_delay();

    gpio_put(dev->cs_pin, 0);
    
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[0], 1);
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[1], 1);
    spi_read_blocking(DAC81408_SPI_INSTANCE, 0, &buf[2], 1);

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
    // Preserve bits 2-5 (differential mode enables) and RESERVED bits
    uint16_t mask_preserve = 0x3C7F;  // Keep bits 2-5, 7, 8-13, 15
    uint16_t ref_bit = (state == DAC81408_REF_OFF) ? (1 << 14) : 0;
    
    dev->genconfig_cache = (dev->genconfig_cache & mask_preserve) | ref_bit;
    dac81408_write_register(dev, DAC81408_REG_GENCONFIG, dev->genconfig_cache);
}

int dac81408_get_int_reference(dac81408_t *dev)
{
    uint16_t res = dac81408_read_register(dev, DAC81408_REG_GENCONFIG);
    return (res & (1 << 14)) ? DAC81408_REF_OFF : DAC81408_REF_ON;
}


void dac81408_set_range(dac81408_t *dev, int ch, dac81408_range_t range) {
    if (ch < 0 || ch > 7) return;
    
    uint8_t local_ch = ch & 0x3;  // 0-3 index within register
    uint16_t mask = 0xF << (4 * local_ch);
    
    if (ch < 4) {
        dev->dacrange0_cache = (dev->dacrange0_cache & ~mask) | ((range << (4 * local_ch)) & mask);
        dac81408_write_register(dev, DAC81408_REG_DACRANGE0, dev->dacrange0_cache);
    } else {
        dev->dacrange1_cache = (dev->dacrange1_cache & ~mask) | ((range << (4 * local_ch)) & mask);
        dac81408_write_register(dev, DAC81408_REG_DACRANGE1, dev->dacrange1_cache);
    }
}

int dac81408_get_range(dac81408_t *dev, int ch) {
    if (ch < 0 || ch > 7) return -1;
    uint8_t local_ch = ch & 0x3;
    uint16_t cache = (ch < 4) ? dev->dacrange0_cache : dev->dacrange1_cache;
    return (cache >> (4 * local_ch)) & 0xF;
}

void dac81408_set_out(dac81408_t *dev, int ch, uint16_t val)
{
    if (ch < 0 || ch > 7) return;
    
    uint8_t address = DAC81408_REG_DAC0 + ch;
    dac81408_write_register(dev, address, val);
}

uint16_t dac81408_get_out(dac81408_t *dev, uint8_t reg)
{
    uint16_t val = dac81408_read_register(dev, reg);

    return val;
}

void dac81408_set_sync(dac81408_t *dev, int ch, dac81408_sync_t mode)
{
    if (ch < 0 || ch > 7) return;
    
    // SYNC-EN bits are at positions 4-11
    if (mode == DAC81408_SYNC_LDAC) {
        dev->syncconfig_cache |= (1UL << (ch + 4));
    } else {
        dev->syncconfig_cache &= ~(1UL << (ch + 4));
    }
    dac81408_write_register(dev, DAC81408_REG_SYNCCONFIG, dev->syncconfig_cache);
}

void dac81408_trigger_ldac(dac81408_t *dev)
{
    if (dev->ldac_pin == 0) return;
    
    gpio_put(dev->ldac_pin, 0);   // Assert LDAC (active low)
    _dac81408_tldac_delay();                  
    gpio_put(dev->ldac_pin, 1);   // De-assert
}
