#include <at89x051.h>

#define LED P3_7

// wait for 10msec with Timer0
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - for 10msec = 9216 times
// - timer value = 65536 - 9216 = 56320 = 0xdc00
inline void delay10ms(void)
{
  TMOD = 0x01; // timer 0 mode 1
  TH0 = 0xdc;  // for 10msec
  TL0 = 0x00;
  TR0 = 1; // start
  while (TF0 == 0)
  { // check overflow
    __asm NOP __endasm;
  }
  TR0 = 0; // stop
  TF0 = 0; // clear overflow
}

// delay in 10msec unit
void delay10(int tmsec)
{
  int i;
  for (i = 0; i < tmsec; ++i)
  {
    delay10ms();
  }
}

void main()
{
  while (1)
  {
    LED = 1;
    delay10(50);
    LED = 0;
    delay10(50);
  }
}
