#include "configuration_bits.c"
#include "xc.h"
#include "ds18b20.h"

#define ONE_WIRE_TRIS TRISBbits.TRISB2

#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

#define FCY 32000000UL/16
#include <libpic30.h>

void main(void)
{    
    ANSBbits.ANSB2 = 0;
    ONE_WIRE_TRIS = 1;
    
    unsigned char i;
    unsigned char scratchpad[9];
    
    ow_reset_pulse();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(CONVERT_T);
    
    while (!read_ow())
    {
        // Wait for ds18b20 to finish temperature conversion
    }
    
    ow_reset_pulse();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(READ_SCRATCHPAD);
    
    for (i = 0; i < 9; i++)
        scratchpad[i] = ow_read_byte();
    
    while(1)
    {
    }
}
