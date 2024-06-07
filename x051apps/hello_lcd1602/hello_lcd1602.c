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

void message(uint8_t write)
{
  if (write)
    lcd1602_puts("World!");
  else
    lcd1602_puts("Hello!");
}

void main()
{
  lcd1602_init();
  lcd1602_display(1, 1, 1);
  delay10(10);

  uint8_t write = 0;
  uint8_t clear = 3;

  while (1)
  {
    BLINK_LED = 1;
    delay10(50);

    BLINK_LED = 0;
    delay10(50);

    clear--;
    if (clear == 0)
    {
      lcd1602_clear();
      clear = 3;
    }
    else
    {
      lcd1602_move(write, 0);
      message(write);
      write = 1 - write;
    }
  }
}
