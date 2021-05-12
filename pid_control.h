#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define RELAY_LAT LATBbits.LATB3

struct pid_control
{
    float set_point;
    bool heating_on;
};

void initialize_pid_control();
uint8_t adjust_set_point_up(struct pid_control *pid);
uint8_t adjust_set_point_down(struct pid_control *pid);
void toggle_relay(struct pid_control *pid, float temp);
