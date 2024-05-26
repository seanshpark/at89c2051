#include <at89x051.h>

#define LED P1_7

// wait for 1msec with Timer0
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - timer value = 65536 - 921 = 64614 = 0xfc66
void delay1ms(void)
{
    TMOD = 0x01;        // timer 0 mode 1
    TH0 = 0xfc;         // for 1msec
    TL0 = 0x66;
    TR0 = 1;            // start
    while (TF0 == 0)    // check overflow
        ;
    TR0 = 0;            // stop
    TF0 = 0;            // clear overflow
}

void delay(int msec)
{
    int i;
    for (i = 0; i < msec; ++i) {
        delay1ms();
    }
}

void main()
{
    while(1) {
        LED = 1;
        delay(500);
        LED = 0;
        delay(500);
    }
}
