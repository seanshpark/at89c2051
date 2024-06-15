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

uint8_t t_20;

uint8_t t_up;
uint8_t t_dn;

void int0_isr(void) __interrupt(0) __using(1)
{
  if (P3_3)
    t_dn++;
  else
    t_up++;
}

// 10msec with Timer1
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - for 10msec = 9216 times -> 65536 - 9216 = 56320 = 0xdc00
// - for 50msec = 46080 times -> 65536 - 46080 = 19456 = 0x4c00
void timer1_isr(void) __interrupt(3) __using(2)
{
  TH1 = 0x4c;
  TL1 = 0x00;

  t_20++;
  if (t_20 > 19)
    t_20 = 0;
}

void timer1_start(void)
{
  TH1 = 0x4c;
  TL1 = 0x00;
  TR1 = 1; // start
}

//  0 ~ 99 to "00 ~ 99"
void num2asii(uint8_t num, char *str)
{
  uint8_t d10;
  uint8_t d01;
  if (num > 99)
    num = 99;
  d10 = num / 10;
  d01 = num % 10;
  str[0] = d10 + '0';
  str[1] = d01 + '0';
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

void main()
{
  uint8_t b_up, b_dn;
  uint8_t b_20;
  uint1_t update;
  char str_nums[10];

  delay10(10);
  lcd1602_i2c_init(1, 0x27);
  lcd1602_i2c_display(1, 1, 1);
  delay10(10);

  TMOD = 0x11; // timer 0/1: mode 1 (16bit mode)
  PX0 = 1;     // high priority for Int0
  PT0 = 1;     // high priority for Timer0
  PT1 = 1;     // high priority for Timer1
  IT0 = 1;     // Int0 active falling edge
  EA = 1;      // enable interrupt
  ET1 = 1;     // Timer1 use interrupt
  EX0 = 1;     // Int0 use interrupt

  t_dn = t_up = 0;
  b_up = b_dn = 1;

  t_20 = 0;
  b_20 = 0;
  timer1_start();

  // forat as "00:00"
  str_nums[2] = ':';
  str_nums[5] = 0;

  while (1)
  {
    uint1_t led = (t_20 < 10) ? 1 : 0;
    BLINK_LED = led;
    if (b_20 != t_20)
    {
      b_20 = t_20;
      if (sw_p3_4())
        t_dn++;
      if (sw_p3_5())
        t_up++;
    }

    EX0 = 0;
    if (t_dn > 99)
      t_dn = 0;
    if (t_up > 99)
      t_up = 0;
    update = (t_up != b_up || t_dn != b_dn);
    if (update)
    {
      b_dn = t_dn;
      b_up = t_up;
    }
    EX0 = 1;

    if (update)
    {
      num2asii(b_dn, &str_nums[0]);
      num2asii(b_up, &str_nums[3]);
      lcd1602_i2c_move(0, 0);
      lcd1602_i2c_puts(str_nums);
    }
  }
}
