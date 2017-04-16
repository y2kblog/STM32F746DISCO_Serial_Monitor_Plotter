#include "mbed.h"

//------------------------------------
// Hyperterminal configuration
// 115200 bauds, 8-bit data, no parity
//------------------------------------

Serial pc(SERIAL_TX, SERIAL_RX);

int main()
{
    float time = 0.0f;
    float sinval, cosval;
    pc.baud(115200);
    
    while(1) {
        wait(0.01);
        sinval = sinf(6.283f * time);
        cosval = cosf(6.283f * time);
        
        /*pc.printf("%d,%d,%d,%d,%d,%d,%d,%d\n",
         (int16_t)(100.0f*sinval), (int16_t)(100.0f*cosval),
         (int16_t)(70.0f*sinval),  (int16_t)(70.0f*cosval),
         (int16_t)(50.0f*sinval),  (int16_t)(50.0f*cosval),
         (int16_t)(30.0f*sinval),  (int16_t)(30.0f*cosval)
         );*/
        pc.printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
         100.0f*sinval, 100.0f*cosval,
          70.0f*sinval,  70.0f*cosval,
          50.0f*sinval,  50.0f*cosval,
          30.0f*sinval,  30.0f*cosval
         );
        time += 0.01f;
    }
}
