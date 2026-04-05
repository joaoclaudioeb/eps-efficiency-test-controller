/**
@file ads1256.c
@brief Source file for controlling TI's 24-bit ADC, the ADS1256.
@details This code was based on CuriousScientist0's one (https://github.com/CuriousScientist0/ADS1256/blob/main/src/ADS1256.c and https://www.youtube.com/watch?v=GBWJdyjRIdM&t=262s).
@author João Cláudio Elsen Barcellos
@version 0.0.1
@date 29/03/2026
*/

#include "ads1256.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#ifndef ADS1256_SPI_INSTANCE
#define ADS1256_SPI_INSTANCE spi1
#endif

#ifndef ADS1256_SPI_BAUDRATE
#define ADS1256_SPI_BAUDRATE 8000000
#endif

#define BIT_WRITE(var, bit, val)                \
    do {                                        \
        if (val) (var) |= (1U << (bit));        \
        else (var) &= ~(1U << (bit));           \
    } while(0)

void ads1256_set_drate(ads1256_t *dev, ads1256_drate_t drate)
{
    ads1256_write_register(dev, ADS1256_REG_ADDR_DRATE, drate);
    dev->drate_reg = drate;
    sleep_ms(200);
}

void ads1256_set_mux(ads1256_t *dev, ads1256_mux_t mux)
{
    ads1256_write_register(dev, ADS1256_REG_ADDR_MUX, mux);
    dev->mux_reg = mux;
    sleep_ms(200);
}

void ads1256_set_pga(ads1256_t *dev, ads1256_pga_t pga)
{
    dev->adcon_reg = dev->adcon_reg | 0x07;
    dev->adcon_reg = dev->adcon_reg & pga;
    
    ads1256_write_register(dev, ADS1256_REG_ADDR_ADCON, dev->adcon_reg);
    sleep_ms(200);
}

void ads1256_set_clkout(ads1256_t *dev, uint8_t clkout)
{
    if (clkout == 0) {
        dev->adcon_reg &= ~(1U << 6);
        dev->adcon_reg &= ~(1U << 5);
    } else if (clkout == 1) {
        dev->adcon_reg &= ~(1U << 6);
        dev->adcon_reg |= (1U << 5);
    } else if (clkout == 2) {
        dev->adcon_reg |= (1U << 6);
        dev->adcon_reg &= ~(1U << 5);
    } else if (clkout == 3) {
        dev->adcon_reg |= (1U << 6);
        dev->adcon_reg |= (1U << 5);
    } else {
    }

    ads1256_write_register(dev, ADS1256_REG_ADDR_ADCON, dev->adcon_reg);
    sleep_ms(100);
}

void ads1256_set_sdcs(ads1256_t *dev, uint8_t sdcs)
{
    if (sdcs == 0) {
        dev->adcon_reg &= ~(1U << 4);
        dev->adcon_reg &= ~(1U << 3);
    } else if (sdcs == 1) {
        dev->adcon_reg &= ~(1U << 4);
        dev->adcon_reg |= (1U << 3);
    } else if (sdcs == 2) {
        dev->adcon_reg |= (1U << 4);
        dev->adcon_reg &= ~(1U << 3);
    } else if (sdcs == 3) {
        dev->adcon_reg |= (1U << 4);
        dev->adcon_reg |= (1U << 3);
    } else {
    }

    ads1256_write_register(dev, ADS1256_REG_ADDR_ADCON, dev->adcon_reg);
    sleep_ms(100);
}

void ads1256_set_byte_order(ads1256_t *dev, uint8_t byte_order)
{
    if (byte_order == 0) {
        // Byte order is MSB (default)
        dev->status_reg &= ~(1U << 3);
    } else if (byte_order == 1) {
        // Byte order is LSB
        dev->status_reg |= (1U << 3);
    } else {
    }

    ads1256_write_register(dev, ADS1256_REG_ADDR_STATUS, dev->status_reg);
    sleep_ms(100);
}

void ads1256_set_autocal(ads1256_t *dev, uint8_t acal)
{
    if (acal == 0) {
        // Auto-calibration is disabled (default)
        dev->status_reg &= ~(1U << 2);
    } else if (acal == 1) {
        // Auto-calibration is enabled
        dev->status_reg |= (1U << 2);
    } else {
    }

    ads1256_write_register(dev, ADS1256_REG_ADDR_STATUS, dev->status_reg);
    sleep_ms(100);
}

void ads1256_set_buffer(ads1256_t *dev, uint8_t bufen)
{
    if (bufen == 0) {
        // Analog input buffer is disabled (default)
        dev->status_reg &= ~(1U << 1);
    } else if (bufen == 1) {
        // Analog input buffer is enabled (recommended)
        dev->status_reg |= (1U << 1);
    } else {
    }

    ads1256_write_register(dev, ADS1256_REG_ADDR_STATUS, dev->status_reg);
    sleep_ms(100);
}

void ads1256_set_gpio_direction(ads1256_t *dev, uint8_t dir0, uint8_t dir1, uint8_t dir2, uint8_t dir3)
{
    // Default: 11100000 - DEC: 224 - Ref: p32 I/O section
    // Sets D3-D0 as input or output
    uint8_t gpio_bit7, gpio_bit6, gpio_bit5, gpio_bit4;

    // Bit7: DIR3
    if (dir3 == 1) {
        gpio_bit7 = 1;  // D3 is input (default)
    } else {
        gpio_bit7 = 0;  // D3 is output
    }
    if (gpio_bit7) dev->gpio_reg |= (1U << 7); else dev->gpio_reg &= ~(1U << 7);
    
    // Bit6: DIR2
    if (dir2 == 1) {
        gpio_bit6 = 1;  // D2 is input (default)
    } else {
        gpio_bit6 = 0;  // D2 is output
    }
    if (gpio_bit6) dev->gpio_reg |= (1U << 6); else dev->gpio_reg &= ~(1U << 6);
    
    // Bit5: DIR1
    if (dir1 == 1) {
        gpio_bit5 = 1;  // D1 is input (default)
    } else {
        gpio_bit5 = 0;  // D1 is output
    }
    if (gpio_bit5) dev->gpio_reg |= (1U << 5); else dev->gpio_reg &= ~(1U << 5);
    
    // Bit4: DIR0
    if (dir0 == 1) {
        gpio_bit4 = 1;  // D0 is input
    } else {
        gpio_bit4 = 0;  // D0 is output (default)
    }
    if (gpio_bit4) dev->gpio_reg |= (1U << 4); else dev->gpio_reg &= ~(1U << 4);


    ads1256_write_register(dev, ADS1256_REG_ADDR_IO, dev->gpio_reg);
    sleep_ms(100);
}

void ads1256_write_gpio(ads1256_t *dev, uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3)
{
    // Sets D3-D0 output values
    // It is important that first one must use setGPIO, then writeGPIO

    uint8_t gpio_bit3, gpio_bit2, gpio_bit1, gpio_bit0;

    // Bit3: D3
    if (gpio3 == 1) {
        gpio_bit3 = 1;
    } else {
        gpio_bit3 = 0;
    }
    if (gpio_bit3) dev->gpio_reg |= (1U << 3); else dev->gpio_reg &= ~(1U << 3);
    
    // Bit2: D2
    if (gpio2 == 1) {
        gpio_bit2 = 1;
    } else {
        gpio_bit2 = 0;
    }
    if (gpio_bit2) dev->gpio_reg |= (1U << 2); else dev->gpio_reg &= ~(1U << 2);
    
    // Bit1: D1
    if (gpio1 == 1) {
        gpio_bit1 = 1;
    } else {
        gpio_bit1 = 0;
    }
    if (gpio_bit1) dev->gpio_reg |= (1U << 1); else dev->gpio_reg &= ~(1U << 1);
    
    // Bit0: D0
    if (gpio0 == 1) {
        gpio_bit0 = 1;
    } else {
        gpio_bit0 = 0;
    }
    if (gpio_bit0) dev->gpio_reg |= (1U << 0); else dev->gpio_reg &= ~(1U << 0);
    

    ads1256_write_register(dev, ADS1256_REG_ADDR_IO, dev->gpio_reg);
    sleep_ms(100);
}

int8_t ads1256_read_gpio(ads1256_t *dev, uint8_t gpio_pin)
{
    uint8_t gpio_bit3, gpio_bit2, gpio_bit1, gpio_bit0;

    dev->gpio_reg = ads1256_read_register(dev, ADS1256_REG_ADDR_IO);  // Read the GPIO register

    // Save each bit values in a variable
    gpio_bit3 = (dev->gpio_reg >> 3) & 1;
    gpio_bit2 = (dev->gpio_reg >> 2) & 1;
    gpio_bit1 = (dev->gpio_reg >> 1) & 1;
    gpio_bit0 = (dev->gpio_reg >> 0) & 1;

    sleep_ms(100);

    // Return the selected bit value
    // Bit3: D3
    if (gpio_pin == 0) {
        return gpio_bit0;
    }
    // Bit2: D2
    if (gpio_pin == 1) {
        return gpio_bit1;
    }
    // Bit1: D1
    if (gpio_pin == 2) {
        return gpio_bit2;
    }
    // Bit0: D0
    if (gpio_pin == 3) {
        return gpio_bit3;
    }

    return -1;
}

float ads1256_convert_to_voltage(ads1256_t *dev, int32_t raw)
{
    // if the 24th digit (sign) is 1, the number is negative
    if ((raw >> 23) == 1) {
        raw = raw - 16777216;  // conversion for the negative sign, "mirroring" around zero
    }
    
    // 8388608 = 2^23
    // PGA gain is stored in bits 0-2 of ADCON register (extract with & 0x07)
    float voltage = ((2.0f * dev->vref) / 8388608.0f) * (float)raw / powf(2.0f, (dev->adcon_reg & 0x07));
    
    return voltage;
}



uint8_t ads1256_read_register(ads1256_t *dev, uint8_t reg)
{
    // Configurar formato SPI (equivalente a SPI.beginTransaction)
    spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    
    gpio_put(dev->cs_pin, 0);  // CS LOW - "CS must stay LOW during the entire sequence"
    
    // Send RREG command: 0x10 | registerAddress
    uint8_t cmd = 0x10 | reg;
    uint8_t dummy;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
    
    // Send 2nd (empty) command byte
    cmd = 0x00;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
    
    sleep_us(5);  // see t6 in the datasheet
    
    // Read out the register value (send 0xFF, receive value)
    cmd = 0xFF;
    uint8_t value;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &value, 1);
    
    gpio_put(dev->cs_pin, 1);  // CS HIGH
    sleep_ms(100);
    
    return value;
}

int32_t ads1256_read_continuous(ads1256_t *dev)
{
    if (dev->is_acquisition_running == false) {
        dev->is_acquisition_running = true;
        
        // Configurar formato SPI
        spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
        
        gpio_put(dev->cs_pin, 0);  // CS LOW
        
        // Wait for DRDY to go LOW
        while (gpio_get(dev->drdy_pin)) {
            tight_loop_contents();
        }
        
        // Issue RDATAC command (0x03)
        uint8_t cmd = 0x03;
        uint8_t dummy;
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        sleep_us(7);  // Wait t6 time
        // ← Nota: CS permanece LOW, igual ao original!
    } else {
        // Wait for DRDY to go LOW
        while (gpio_get(dev->drdy_pin)) {
            tight_loop_contents();
        }
    }
    
    // Read 3 bytes (24-bit data) - CS ainda está LOW
    uint8_t msb, mid, lsb;
    
    uint8_t cmd = 0x00;
    uint8_t dummy;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &msb, 1);
    
    cmd = 0x00;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &mid, 1);
    
    cmd = 0x00;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &lsb, 1);
    
    // Combine into 24-bit value
    int32_t value = ((int32_t)msb << 16) | ((int32_t)mid << 8) | lsb;
    
    // ← Nota crítica: NÃO colocamos CS HIGH aqui!
    // Igual ao original: mantém CS LOW para leituras contínuas subsequentes.
    // Para parar, o usuário deve chamar ads1256_stop().
    
    return value;
}

void ads1256_stop_continuous(ads1256_t *dev)
{
    spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    
    gpio_put(dev->cs_pin, 0);
    
    uint8_t cmd = 0x0F;
    uint8_t dummy;
    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
    
    gpio_put(dev->cs_pin, 1);
    
    dev->is_acquisition_running = false;
}

int32_t ads1256_cycle_single(ads1256_t *dev)
{
    uint8_t cmd;
    uint8_t dummy;
    if (dev->is_acquisition_running == false) {
        dev->is_acquisition_running = true;
        dev->cycle = 0;
        
        // Configurar SPI
        spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
        
        gpio_put(dev->cs_pin, 0);  // CS LOW
        
        // Write MUX register: AIN0+AINCOM (SING_0 = 0x0F)
        cmd = 0x50 | 0x01;  // WREG | MUX_REG
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        cmd = 0x00;  // 2nd empty command byte
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        cmd = 0x0F;  // SING_0 value
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        gpio_put(dev->cs_pin, 1);  // CS HIGH
        sleep_ms(50);
        
        gpio_put(dev->cs_pin, 0);  // CS LOW again for continuous operation
    } else {
        // else vazio, igual ao original
    }

    if (dev->cycle < 8) {
        dev->output_value = 0;
        
        // Wait for DRDY low
        while (gpio_get(dev->drdy_pin)) {
            tight_loop_contents();
        }
        
        // Step 1: Update MUX based on cycle
        switch (dev->cycle) {
            case 0:  // Channel 2 (AIN1+AINCOM)
                {
                    cmd = 0x50 | 0x01;  // WREG | MUX_REG
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x1F;  // SING_1
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 1:  // Channel 3 (AIN2+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x2F;  // SING_2
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 2:  // Channel 4 (AIN3+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x3F;  // SING_3
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 3:  // Channel 5 (AIN4+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x4F;  // SING_4
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 4:  // Channel 6 (AIN5+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x5F;  // SING_5
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 5:  // Channel 7 (AIN6+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x6F;  // SING_6
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 6:  // Channel 8 (AIN7+AINCOM)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x7F;  // SING_7
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 7:  // Channel 1 (AIN0+AINCOM) - wrap around
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x0F;  // SING_0
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
        }
        
        // Step 2: SYNC + WAKEUP sequence
        cmd = 0xFC;  // SYNC command
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        sleep_us(4);  // t11 delay
        cmd = 0xFF;  // WAKEUP command
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        // Step 3: Issue RDATA command
        cmd = 0x01;  // RDATA
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        sleep_us(7);  // t6 delay
        
        // Read 3 bytes (MSB, mid, LSB)
        uint8_t msb, mid, lsb;
        cmd = 0x0F;  // Original usa 0x0F aqui
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &msb, 1);
        cmd = 0x0F;
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &mid, 1);
        cmd = 0x0F;
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &lsb, 1);
        
        // Combine into 24-bit value
        dev->output_value = ((int32_t)msb << 16) | ((int32_t)mid << 8) | lsb;
        
        dev->cycle++;  // Increase cycle
        if (dev->cycle == 8) {
            dev->cycle = 0;  // Reset to restart from channel 1
        }
    }
    
    return dev->output_value;
}

int32_t ads1256_cycle_differential(ads1256_t *dev)
{
    uint8_t cmd;
    uint8_t dummy;
    if (dev->is_acquisition_running == false) {
        dev->cycle = 0;
        dev->is_acquisition_running = true;
        
        // Configurar SPI
        spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
        
        // Set AIN0+AIN1 as inputs manually
        gpio_put(dev->cs_pin, 0);  // CS LOW
        
        cmd = 0x50 | 0x01;  // WREG | MUX_REG
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        cmd = 0x00;  // 2nd empty command byte
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        cmd = 0x01;  // DIFF_0_1 value
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        gpio_put(dev->cs_pin, 1);  // CS HIGH
        sleep_ms(50);
        
        gpio_put(dev->cs_pin, 0);  // CS LOW again for continuous operation
    } else {
        // else vazio, igual ao original
    }

    if (dev->cycle < 4) {
        dev->output_value = 0;
        
        // Wait for DRDY low
        while (gpio_get(dev->drdy_pin)) {
            tight_loop_contents();
        }
        
        // Step 1: Update MUX based on cycle
        switch (dev->cycle) {
            case 0:  // Channel 2 (AIN2+AIN3)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x23;  // DIFF_2_3
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 1:  // Channel 3 (AIN4+AIN5)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x45;  // DIFF_4_5
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 2:  // Channel 4 (AIN6+AIN7)
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x67;  // DIFF_6_7
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
            case 3:  // Channel 1 (AIN0+AIN1) - wrap around
                {
                    cmd = 0x50 | 0x01;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x00;
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                    cmd = 0x01;  // DIFF_0_1
                    spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
                }
                break;
        }
        
        // Step 2: SYNC + WAKEUP sequence
        cmd = 0xFC;  // SYNC command
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        sleep_us(4);  // t11 delay
        cmd = 0xFF;  // WAKEUP command
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        
        // Step 3: Issue RDATA command
        cmd = 0x01;  // RDATA
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &dummy, 1);
        sleep_us(7);  // t6 delay
        
        // Read 3 bytes (MSB, mid, LSB)
        uint8_t msb, mid, lsb;
        cmd = 0x00;  // Original usa 0x00 aqui
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &msb, 1);
        cmd = 0x00;
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &mid, 1);
        cmd = 0x00;
        spi_write_read_blocking(ADS1256_SPI_INSTANCE, &cmd, &lsb, 1);
        
        // Combine into 24-bit value
        dev->output_value = ((int32_t)msb << 16) | ((int32_t)mid << 8) | lsb;
        
        dev->cycle++;
        if (dev->cycle == 4) {
            dev->cycle = 0;  // Reset to restart from channel 1
        }
    }
    
    return dev->output_value;
}

// =============================

void ads1256_wait_drdy(ads1256_t *dev)
{
    while (gpio_get(dev->drdy_pin)){
        tight_loop_contents();
    }
}

void ads1256_sync(ads1256_t *dev)
{
    uint8_t cmd = ADS1256_CMD_SYNC;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
    sleep_us(4);
}

void ads1256_wakeup(ads1256_t *dev)
{
    uint8_t cmd = ADS1256_CMD_WAKEUP;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
}

int32_t ads1256_read_single(ads1256_t *dev)
{
    uint8_t cmd = ADS1256_CMD_RDATA;

    while (gpio_get(dev->drdy_pin)){
        tight_loop_contents();
    }
    
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
    
    sleep_us(7);  // Wait t6 time (~6.51 us)
    
    uint8_t msb, mid, lsb = 0;
    spi_read_blocking(ADS1256_SPI_INSTANCE, 0, &msb, 1);
    spi_read_blocking(ADS1256_SPI_INSTANCE, 0, &mid, 1);    
    spi_read_blocking(ADS1256_SPI_INSTANCE, 0, &lsb, 1);
    
    // Combine into 24-bit value
    int32_t value = ((int32_t)msb << 16) | ((int32_t)mid << 8) | lsb;
    
    return value;
}

void ads1256_start_communication(ads1256_t *dev)
{
    spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_put(dev->cs_pin, 0);
}

void ads1256_end_communication(ads1256_t *dev)
{
    gpio_put(dev->cs_pin, 1);
}

void ads1256_write_register(ads1256_t *dev, uint8_t reg, uint8_t value)
{
    while (gpio_get(dev->drdy_pin)){
        tight_loop_contents();
    }
    
    spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_put(dev->cs_pin, 0);
    sleep_us(5);  // See t6 in the datasheet
    
    // Send WREG command: 0x50 | registerAddress
    uint8_t cmd = 0x50 | reg;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
    
    // Send 2nd (empty) command byte
    cmd = 0x00;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
    
    // Pass the value to the register
    cmd = value;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &cmd, 1);
    
    gpio_put(dev->cs_pin, 1);
    sleep_ms(100);
}

void ads1256_send_command(ads1256_t *dev, uint8_t cmd)
{
    while (gpio_get(dev->drdy_pin)){
        tight_loop_contents();
    }
    
    spi_set_format(ADS1256_SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST); 
    gpio_put(dev->cs_pin, 0);
    sleep_us(5);
    
    uint8_t command = cmd;
    spi_write_blocking(ADS1256_SPI_INSTANCE, &command, 1);
    
    sleep_us(5);
    gpio_put(dev->cs_pin, 1);
}

void ads1256_init(ads1256_t *dev,
                  uint8_t drdy_pin,
                  uint8_t reset_pin,
                  uint8_t sync_pin,
                  uint8_t cs_pin,
                  uint8_t miso_pin,
                  uint8_t sclk_pin,
                  uint8_t mosi_pin,
                  float vref)
{    
    if (cs_pin != 0) {
        dev->cs_pin = cs_pin;
        gpio_init(cs_pin);
        gpio_set_dir(cs_pin, GPIO_OUT);
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

    dev->drdy_pin = drdy_pin;
    gpio_init(drdy_pin);
    gpio_set_dir(drdy_pin, GPIO_IN);
    
    if (reset_pin != 0) {
        dev->reset_pin = reset_pin;
        gpio_init(reset_pin);
        gpio_set_dir(reset_pin, GPIO_OUT);
    }
    if (sync_pin != 0) {
        dev->sync_pin = sync_pin;
        gpio_init(sync_pin);
        gpio_set_dir(sync_pin, GPIO_OUT);
    }
    
    dev->vref = vref;
}

void ads1256_initialize(ads1256_t *dev)
{
    // SPI config.
    spi_init(ADS1256_SPI_INSTANCE, ADS1256_SPI_BAUDRATE);
    gpio_set_function(dev->mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(dev->miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(dev->sclk_pin, GPIO_FUNC_SPI);
    gpio_put(dev->cs_pin, 1);

    // Force ADS1256 to reset...
    gpio_put(dev->reset_pin, 0);
    sleep_ms(200);
    gpio_put(dev->reset_pin, 1);

    // ...and then make a command-based reset
    ads1256_send_command(dev, ADS1256_CMD_RESET);
    sleep_ms(200);
    
    // Regs. config.
    // STATUS: BUFEN=1, ACAL=1, BITORDER=MSB (0b00110110 = 0x36)
    ads1256_write_register(dev, ADS1256_REG_ADDR_STATUS, 0x36);
    sleep_ms(200);
    
    // ADCON: PGA=64, CLKOUT=OFF, SDCS=OFF (0b00000111 = 0x07)
    ads1256_write_register(dev, ADS1256_REG_ADDR_ADCON, 0x07);
    sleep_ms(200);
    
    // DRATE: 10 SPS (0b00100011 = 0x23)
    ads1256_write_register(dev, ADS1256_REG_ADDR_DRATE, 0x23);
    sleep_ms(200);
}
