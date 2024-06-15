/*
 * Copyright 2024 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <at89x051.h>

typedef unsigned char uint8_t;
typedef __bit uint1_t;

#define BLINK_LED P3_7

uint8_t timer1_cnt;

// timer 1 is int 3
void timer1_isr(void) __interrupt(3) __using(1)
{
  TH1 = 0xdc; // for 10msec
  TL1 = 0x00;
  if (timer1_cnt)
    timer1_cnt--;
}

// 10msec with Timer1
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - for 10msec = 9216 times
// - timer value = 65536 - 9216 = 56320 = 0xdc00
void timer1_start(void)
{
  TH1 = 0xdc;
  TL1 = 0x00;
  TR1 = 1; // start
}

void main()
{
  uint8_t tcnt = 0;
  uint1_t led = 1;

  TMOD = 0x11; // timer 0/1: mode 1 (16bit mode)
  EA = 1;
  ET1 = 1; // timer1 use interrupt

  BLINK_LED = led;

  timer1_cnt = 50; // make 10ms x 50 -> 500ms
  timer1_start();

  while (1)
  {
    if (timer1_cnt == 0)
    {
      led ^= 1;
      BLINK_LED = led;
      timer1_cnt = 50;
    }
  }
}
