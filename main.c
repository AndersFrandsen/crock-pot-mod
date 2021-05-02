#include "xc.h"
#include "mcc_generated_files/system.h"
#include "ds18b20.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define RELAY_LAT LATBbits.LATB3

#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

#define FCY 32000000UL / 16
#include <libpic30.h>

void main(void)
{
    SYSTEM_Initialize();
    
    ANSBbits.ANSB3 = 0;
    ANSBbits.ANSB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    uint8_t count = 0;
    char celcius[16] = "  C";
    celcius[1] = 0xDF;
    
    lcd_setContrast(80);
    //lcd_writeString("-- °C            ", 0);
    lcd_writeString("On (01)          ", 1);

    while (1)
    {
        unsigned char i;
        uint8_t scratchpad[9];

        ow_reset_pulse();
        ow_write_byte(SKIP_ROM);
        ow_write_byte(CONVERT_T);

        while (!read_ow()) {
            // Wait for ds18b20 to finish temperature conversion
        }

        ow_reset_pulse();
        ow_write_byte(SKIP_ROM);
        ow_write_byte(READ_SCRATCHPAD);
        
        for (i = 0; i < 9; i++)
            scratchpad[i] = ow_read_byte();
        
        uint16_t temp = ((scratchpad[1] << 8) | (scratchpad[0])) / 16;
        
        if (temp < 53 && RELAY_LAT == 1) {
            RELAY_LAT = 0;
        }
        else if (temp > 53 && RELAY_LAT == 0) { 
           RELAY_LAT = 1;
        }
        
        char display[16] = "";
        utoa(display, temp, 10);
        strcat(display, celcius);
        lcd_writeString(display, 0);
        
        __delay_ms(10000UL);
    }
}
