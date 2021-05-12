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

struct ds18b20 ds_sensor;
struct pid_control pid;
uint8_t lcd_string_0[16] = "Current        C";
uint8_t lcd_string_1[16] = "Set temp       C";

void interrupt_handler(void)
{
    // Read current temperature and print to LCD
    float temp = get_temp(&ds_sensor);
    utoa(&lcd_string_0[11], (uint8_t)temp, 10);
    lcd_writeString(lcd_string_0, 0);
        
    // Toggle relay on/off and log to RPi over UART
    toggle_relay(&pid, temp); 
    log_data(&pid, temp);
}

int main(void)
{
    SYSTEM_Initialize();
    
    ANSBbits.ANSB3 = 0;
    ANSBbits.ANSB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    TMR1_SetInterruptHandler(interrupt_handler);
    TMR1_Start();
    
    // Construct DS18B20 and set to 12 bit resolution
    ds_sensor.resolution = RES_12BIT;
    
    // First conversion when DS18B20 is initialized is always 85 degrees
    uint16_t _temp = get_temp(&ds_sensor);
    
    // Construct PID and set default set point to 55 degrees
    pid.heating_on = true;
    pid.set_point = 55.0;
    
    // Set LCD contrast and print current/set temp messages
    lcd_setContrast(80);
    lcd_string_0[14] = 0xDF;
    lcd_string_1[14] = 0xDF;
    lcd_writeString(lcd_string_0, 0);
    lcd_writeString(lcd_string_1, 1);
    
    while (1)
    {
        if (BUTTON_1_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_1_GetValue() == 0) {
                uint8_t value = adjust_set_point_down(&pid);
                utoa(&lcd_string_1[11], value, 10);
                lcd_writeString(lcd_string_1, 1);
            }
        }
        
        if (BUTTON_2_GetValue() == 0) {
            __delay_ms(3UL);
            if (BUTTON_2_GetValue() == 0) {
                uint8_t value = adjust_set_point_up(&pid);
                utoa(&lcd_string_1[11], value, 10);
                lcd_writeString(lcd_string_1, 1);
            }
        }
    }
}

void log_data(struct pid_control *pid, float temp)
{    
    printf("Current temp: %f, Set point: %f, Relay: %s\n",
            temp,
            pid->set_point,
            pid->heating_on ? "On" : "Off"
    );
}