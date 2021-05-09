#include "xc.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include "ds18b20.h"
#include "pid_control.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#define FCY 32000000UL / 16
#include <libpic30.h>

int main(void)
{
    SYSTEM_Initialize();
    
    ANSBbits.ANSB3 = 0;
    ANSBbits.ANSB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    // First conversion when DS18B20 is initialized is always 85 degrees
    uint16_t _temp = get_temp();
    
    set_resolution();
    
    // Construct PID and set default set point to 55 degrees
    struct pid_control pid;
    pid.heating_on = false;
    pid.set_point = 55;
    
    // Set LCD contrast and print current/set temp messages
    lcd_setContrast(80);
    uint8_t lcd_string_0[16] = "Current        C";
    char lcd_string_1[16] = "Set temp  55   C";
    lcd_string_0[14] = 0xDF;
    lcd_string_1[14] = 0xDF;
    lcd_writeString(lcd_string_0, 0);
    lcd_writeString(lcd_string_1, 1);
    
    while (1)
    {
        // Read current temperature and print
        float temp = get_temp();
        utoa(&lcd_string_0[10], temp, 10);
        lcd_writeString(lcd_string_0, 0);
        
        // Control relay
        toggle_relay(&pid, temp);
        
        if (BUTTON_1_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_1_GetValue() == 0) {
                uint8_t value = adjust_set_point_down(&pid);
                utoa(&lcd_string_1[10], value, 10);
                lcd_writeString(lcd_string_1, 1);
            }
        }
        
        
        if (BUTTON_2_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_2_GetValue() == 0) {
                uint8_t value = adjust_set_point_up(&pid);
                utoa(&lcd_string_1[10], value, 10);
                lcd_writeString(lcd_string_1, 1);
            }
        }
    }
}
