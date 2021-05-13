#include <stdint.h>

// ROM commands used in DS18B20
#define READ_ROM 0x33
#define SKIP_ROM 0xCC

// Function commands used in DS18B20
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE
#define WRITE_SCRATCHPAD 0x4E

// Thermometer Resolution
#define RES_12BIT 0xFF
#define RES_11BIT 0x5F
#define RES_10BIT 0x3F
#define RES_9BIT 0x1F

struct ds18b20
{
    uint8_t resolution;
};

void initialize_ds18b20(void);
void set_resolution(struct ds18b20 *sensor, uint8_t resolution);
float get_temp(struct ds18b20 *sensor);

// Internal functions
void drive_ow_low(void);
void drive_ow_high(void);
unsigned char read_ow(void);
void ow_write_byte(unsigned char byte);
unsigned char ow_read_byte(void);
unsigned char ow_reset_pulse(void);
void ow_write_bit(unsigned char _bit);
unsigned char ow_read_bit(void);