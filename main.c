#include "xc.h"

#define LED_LD1_LAT LATAbits.LATA9
#define LED_LD1_TRIS TRISAbits.TRISA9

void main(void) {
    
    LED_LD1_LAT = 1;
    LED_LD1_TRIS = 0;
    
    while(1)
    {
        
    }
}
