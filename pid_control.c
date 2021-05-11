#include "pid_control.h"

uint8_t adjust_set_point_up(struct pid_control *pid)
{
    if (pid->set_point == 65) {
        pid->set_point = 45;
        return pid->set_point;
    } else {
        return pid->set_point++;
    }
}

uint8_t adjust_set_point_down(struct pid_control *pid)
{
    if (pid->set_point == 45) {
        pid->set_point = 65;
        return pid->set_point;
    } else {
        return pid->set_point--;
    }
}

void toggle_relay(struct pid_control *pid, float temp)
{
    if (temp < pid->set_point && RELAY_LAT == 1) {
        RELAY_LAT = 0;
        pid->heating_on = true;
    }
    else if (temp > pid->set_point && RELAY_LAT == 0) { 
        RELAY_LAT = 1;
        pid->heating_on = false;
    }
}