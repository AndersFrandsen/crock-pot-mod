#include "xc.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include "ds18b20.h"
#include "pid_control.h"
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

int main(void)
{
    SYSTEM_Initialize();
    
    ANSBbits.ANSB3 = 0;
    ANSBbits.ANSB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    struct pid_control pid;
    pid.heating_on = false;
    pid.set_point = 55;
    
    char celcius[16] = "  C";
    celcius[1] = 0xDF;
    
    lcd_setContrast(80);
    lcd_writeString("On (01)          ", 1);

    while (1)
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
        
        uint16_t temp = ((scratchpad[1] << 8) | (scratchpad[0])) / 16;
        
        if (temp < pid.set_point && RELAY_LAT == 1) {
            RELAY_LAT = 0;
        }
        else if (temp > pid.set_point && RELAY_LAT == 0) { 
           RELAY_LAT = 1;
        }
        
        char display[16] = "";
        utoa(display, temp, 10);
        strcat(display, celcius);
        lcd_writeString(display, 0);
        
        if (BUTTON_1_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_1_GetValue() == 0) {
                uint8_t value = adjust_set_point_down(&pid);
                char display[16] = "";
                lcd_writeString(utoa(display, value, 10), 1);
            }
        }
        
        
        if (BUTTON_2_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_2_GetValue() == 0) {
                uint8_t value = adjust_set_point_up(&pid);
                char display[16] = "";
                lcd_writeString(utoa(display, value, 10), 1);
            }
        }
    }
}
