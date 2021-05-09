#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define RELAY_LAT LATBbits.LATB3

struct pid_control
{
    uint8_t set_point;
    bool heating_on;
};

uint8_t adjust_set_point_up(struct pid_control *pid);
uint8_t adjust_set_point_down(struct pid_control *pid);
void toggle_relay(struct pid_control *pid, float temp);

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif
