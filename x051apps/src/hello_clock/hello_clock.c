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

#include <common.h>

#define BLINK_LED P3_7

uint8_t t_hour, t_min, t_sec, t_100;

// timer 1 is int 3
void timer1_clock(void)
{
  t_100++;
  if (t_100 == 100)
  {
    t_100 = 0;
    t_sec++;
    if (t_sec == 60)
    {
      t_sec = 0;
      t_min++;
      if (t_min == 60)
      {
        t_min = 0;
        t_hour++;
        if (t_hour == 24)
          t_hour = 0;
      }
    }
  }
}

void timer1_isr(void) __interrupt(3) __using(1)
{
  TH1 = 0xdc; // for 10msec
  TL1 = 0x00;
  timer1_clock();
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

//  0 ~ 59 to "00 ~ 59"
void num2asii(uint8_t num, char *str)
{
  uint8_t d10 = num / 10;
  uint8_t d01 = num % 10;
  str[0] = d10 + '0';
  str[1] = d01 + '0';
}

void main()
{
  uint8_t b_sec;
  uint8_t show = 0;
  char str_hour[10];

  delay10(10);

  mcp23017_init(0, 0x20);
  mcp23017_modeA(0b11111111); // A as read mode
  mcp23017_modeB(0b00000000); // B as write mode

  lcd1602_i2c_init(1, 0x27);
  lcd1602_i2c_display(1, 1, 1);
  delay10(10);

  TMOD = 0x11; // timer 0/1: mode 1 (16bit mode)
  EA = 1;
  ET1 = 1; // timer1 use interrupt

  b_sec = 100;
  t_hour = 22;
  t_min = 0;
  t_sec = 0;
  t_100 = 0;
  timer1_start();

  str_hour[2] = ':';
  str_hour[5] = ':';

  while (1)
  {
    uint1_t led = (t_100 < 50) ? 1 : 0;
    uint8_t wb = led ? 0b11111111 : 0b00000000;
    BLINK_LED = led;
    mcp23017_writeB(wb);

    if (b_sec != t_sec)
    {
      b_sec = t_sec;
      num2asii(t_hour, &str_hour[0]);
      num2asii(t_min, &str_hour[3]);
      num2asii(t_sec, &str_hour[6]);
      lcd1602_i2c_move(0, 0);
      lcd1602_i2c_puts(str_hour);
    }
  }
}
