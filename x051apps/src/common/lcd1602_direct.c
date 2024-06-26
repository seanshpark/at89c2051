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

#include "common.h"
#include "lcd1602.h"

void lcd1602_send_cmd(uint8_t cmd)
{
  LCD1602_RS = 0;
  LCD1602_RW = 0;
  LCD1602_DB = cmd;
  // clang-format off
__asm
  nop
  nop
__endasm;
  LCD1602_E = 1;
__asm
  nop
  nop
__endasm;
  // clang-format on
  LCD1602_E = 0;
  delay10us();
}

void lcd1602_send_data(uint8_t data)
{
  LCD1602_RS = 1;
  LCD1602_RW = 0;
  LCD1602_DB = data;
  // clang-format off
__asm
  nop
  nop
__endasm;
  LCD1602_E = 1;
__asm
  nop
  nop
__endasm;
  // clang-format on
  LCD1602_E = 0;
  delay10us();
}

void lcd1602_init(void)
{
  uint8_t cmd;

  LCD1602_RS = 0;
  LCD1602_RW = 0;
  LCD1602_DB = 0;
  LCD1602_E = 0;

  delay10(10); // wait for 100msec

  cmd = LCD1602_CMD_FUNCSET | LCD1602_FUNCSET_8BIT | LCD1602_FUNCSET_2LINES;
  lcd1602_send_cmd(cmd);
  delay100us();

  cmd = LCD1602_CMD_DISPLAY | LCD1602_DISPLAY_ON;
  lcd1602_send_cmd(cmd);
  delay100us();

  cmd = LCD1602_CMD_CLEAR;
  lcd1602_send_cmd(cmd);
  delay10ms();

  cmd = LCD1602_CMD_ENTRY | LCD1602_ENTRY_INC;
  lcd1602_send_cmd(cmd);
  delay10ms();
  // init end
}

void lcd1602_display(__bit on, __bit cursor, __bit blink)
{
  uint8_t cmd;
  cmd = LCD1602_CMD_DISPLAY | LCD1602_DISPLAY_ON | LCD1602_DISPLAY_CUR | LCD1602_DISPLAY_BLINK;
  cmd = cmd | (on ? LCD1602_DISPLAY_ON : 0);
  cmd = cmd | (cursor ? LCD1602_DISPLAY_CUR : 0);
  cmd = cmd | (blink ? LCD1602_DISPLAY_BLINK : 0);
  lcd1602_send_cmd(cmd);
  delay10ms();
}

void lcd1602_clear(void)
{
  uint8_t cmd = LCD1602_CMD_CLEAR;
  lcd1602_send_cmd(cmd);
  delay10ms();
}

void lcd1602_putch(uint8_t ch)
{
  lcd1602_send_data(ch);
  delay100us();
}

void lcd1602_puts(char *str)
{
  while (*str)
  {
    uint8_t ch = (uint8_t)(*str);
    lcd1602_putch(ch);
    str++;
  }
}

void lcd1602_move(uint8_t row, uint8_t col)
{
  // row 0 : 0x00 ~ 0x27
  // row 1 : 0x40 ~ 0x67
  if (row > 0x01)
    row = 0x01; // limit row 0 ~ 1
  if (col > 0x27)
    col = 0x27;

  uint8_t ram_offset = row * 0x40 + col;
  ram_offset &= 0b01111111;

  uint8_t cmd = LCD1602_CMD_DDRAMADDR | ram_offset;
  lcd1602_send_cmd(cmd);
  delay100us();
}
