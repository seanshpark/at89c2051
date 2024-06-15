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

uint8_t t_hour, t_min, t_sec, t_20;
uint8_t t_dn, t_up;
uint8_t t_adj;

enum
{
  TIME_ADJ_OFF = 0,
  TIME_ADJ_HOUR = 1,
  TIME_ADJ_MINUTE = 2,
  TIME_ADJ_SEC = 3,
  TIME_ADJ_END = 4,
};

void int0_isr(void) __interrupt(0) __using(1)
{
  if (P3_3)
    t_dn++;
  else
    t_up++;
}

// timer 1 is int 3
inline void timer1_clock(void)
{
  t_20++;
  if (t_20 == 20)
  {
    t_20 = 0;
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
  TH1 = 0x4c; // for 50msec
  TL1 = 0x00;
  timer1_clock();
}

// 10msec with Timer1
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - for 10msec = 9216 times
// - for 50msec = 46080 times
// - 10msec timer value = 65536 - 9216 = 56320 = 0xdc00
// - 50msec timer value = 65536 - 46080 = 19456 = 0x4c00
void timer1_start(void)
{
  TH1 = 0x4c;
  TL1 = 0x00;
  TR1 = 1; // start
}

// return 1 when release switch
uint1_t sw_p3_4(void)
{
  static uint1_t o_p3_4 = 0;
  uint1_t ret = 0;
  if (!o_p3_4 && P3_4)
    ret = 1;
  o_p3_4 = P3_4;
  return ret;
}

// return 1 when release switch
uint1_t sw_p3_5(void)
{
  static uint1_t o_p3_5 = 0;
  uint1_t ret = 0;
  if (!o_p3_5 && P3_5)
    ret = 1;
  o_p3_5 = P3_5;
  return ret;
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

  lcd1602_i2c_init(1, 0x27);
  lcd1602_i2c_display(1, 0, 0);
  delay10(10);

  TMOD = 0x11; // timer 0/1: mode 1 (16bit mode)
  PX0 = 1;     // high priority for Int0
  PT0 = 1;     // high priority for Timer0
  PT1 = 1;     // high priority for Timer1
  IT0 = 1;     // Int0 active falling edge
  EA = 1;      // enable interrupt
  ET1 = 1;     // timer1 use interrupt
  EX0 = 1;     // Int0 use interrupt

  t_adj = TIME_ADJ_OFF; // time adjust mode
  t_dn = 0;
  t_up = 0;

  b_sec = 100;
  t_hour = 22;
  t_min = 0;
  t_sec = 0;
  t_20 = 0;
  timer1_start();

  str_hour[2] = ':';
  str_hour[5] = ':';

  while (1)
  {
    {
      uint1_t led = (t_20 < 10) ? 1 : 0;
      BLINK_LED = led;
    }
    EX0 = 0;
    if (t_adj && (t_dn || t_up))
    {
      uint8_t b_dn = t_dn;
      uint8_t b_up = t_up;

      t_dn = 0;
      t_up = 0;

      if (t_adj == TIME_ADJ_HOUR)
      {
        t_hour += b_up + 24 - b_dn;
        t_hour %= 24;
      }
      else if (t_adj == TIME_ADJ_MINUTE)
      {
        t_min += b_up + 60 - b_dn;
        t_min %= 60;
      }
      else if (t_adj == TIME_ADJ_SEC)
      {
        t_sec += b_up + 60 - b_dn;
        t_sec %= 60;
      }
    }
    EX0 = 1;

    if (b_sec != t_sec)
    {
      b_sec = t_sec;
      num2asii(t_hour, &str_hour[0]);
      num2asii(t_min, &str_hour[3]);
      num2asii(t_sec, &str_hour[6]);
      lcd1602_i2c_move(0, 0);
      lcd1602_i2c_puts(str_hour);

      uint8_t xpos = 0;
      switch (t_adj)
      {
        case TIME_ADJ_HOUR:
          xpos = 1;
          break;
        case TIME_ADJ_MINUTE:
          xpos = 4;
          break;
        case TIME_ADJ_SEC:
          xpos = 7;
          break;
      }
      if (xpos)
        lcd1602_i2c_move(0, xpos);
    }

    if (sw_p3_4())
    {
      t_dn = 0;
      t_up = 0;
      t_adj = (++t_adj) % TIME_ADJ_END;
      switch (t_adj)
      {
        case TIME_ADJ_OFF:
          lcd1602_i2c_display(1, 0, 0);
          break;
        case TIME_ADJ_HOUR:
          lcd1602_i2c_display(1, 1, 1);
          break;
      }
    }
  }
}
