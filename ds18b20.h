#include <stdint.h>

#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE
#define WRITE_SCRATCHPAD 0x4E

#define RES_12BIT 0xFF
#define RES_11BIT 0x5F
#define RES_10BIT 0x3F
#define RES_9BIT 0x1F

enum resolution {RES_12bit, RES_11bit, RES_10bit, RES_9bit};

void drive_ow_low(void);
void drive_ow_high(void);
unsigned char read_ow(void);
void ow_write_byte(unsigned char byte);
unsigned char ow_read_byte(void);
unsigned char ow_reset_pulse(void);
void ow_write_bit(unsigned char _bit);
unsigned char ow_read_bit(void);
float get_temp(void);
void set_resolution(void);