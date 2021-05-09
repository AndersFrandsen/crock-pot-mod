#include "ds18b20.h"
#include "xc.h"

#define ONE_WIRE_LAT LATBbits.LATB2
#define ONE_WIRE_TRIS TRISBbits.TRISB2
#define ONE_WIRE_PORT PORTBbits.RB2

#define HIGH 1
#define LOW 0
#define OUTPUT 0
#define INPUT 1

#define FCY 32000000UL / 16
#include <libpic30.h>

unsigned char ow_reset_pulse(void)
{
    drive_ow_low();
    __delay_us(480UL);
    drive_ow_high();
    __delay_us(70UL);
    int presense = ONE_WIRE_PORT ? 0 : 1;
    __delay_us(410UL);

    return presense;
}

void drive_ow_low(void)
{
    ONE_WIRE_TRIS = OUTPUT;
    ONE_WIRE_LAT = LOW;
}

void drive_ow_high(void)
{
    ONE_WIRE_TRIS = OUTPUT;
    ONE_WIRE_LAT = HIGH;
}

unsigned char read_ow(void)
{
    ONE_WIRE_TRIS = INPUT;

    return ONE_WIRE_PORT;
}

void ow_write_byte(unsigned char byte)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        ow_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

unsigned char ow_read_byte(void)
{
    unsigned char byte = 0;
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        byte >>= 1;

        if(ow_read_bit())
            byte |= 0x80;
    }
    return byte;
}

void ow_write_bit(unsigned char _bit)
{
    if (_bit)
    {
        drive_ow_low();
        __delay_us(6UL);
        drive_ow_high();
        __delay_us(64UL);
    } else
    {
        drive_ow_low();
        __delay_us(60UL);
        drive_ow_high();
        __delay_us(10UL);
    }
}

unsigned char ow_read_bit(void)
{
    unsigned char _bit;

    drive_ow_low();
    __delay_us(6UL);
    drive_ow_high();
    __delay_us(9UL);
    _bit = read_ow();
    __delay_us(55UL);

    return _bit;
}

float get_temp()
{
    unsigned char i;
    uint8_t scratchpad[9];
    
    ow_reset_pulse();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(CONVERT_T);

    while (!read_ow()) {
        // Wait for DS18B20 to finish temperature conversion
    }

    ow_reset_pulse();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(READ_SCRATCHPAD);
        
    for (i = 0; i < 9; i++)
        scratchpad[i] = ow_read_byte();
        
    uint16_t t1 = ((scratchpad[1] << 8) | (scratchpad[0]));
    
    //TODO: switch case for resolution
    
    float temp = ((scratchpad[1] << 8) | (scratchpad[0])) / 16.0;
    
    return temp;
}

void set_resolution()
{
    ow_reset_pulse();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(WRITE_SCRATCHPAD);
    
    ow_write_byte(0x4B);
    ow_write_byte(0x46);
    ow_write_byte(RES_11BIT);
    
    /*
    switch (resolution)
    {
        case "RES_12BIT":
            ow_write_byte(RES_12BIT);
            break;
        case "RES_11BIT":
            ow_write_byte(RES_11BIT);
            break;
        case "RES_10BIT":
            ow_write_byte(RES_10BIT);
            break;
        case "RES_9BIT":
            ow_write_byte(RES_9BIT);
            break;
    }
    */
}