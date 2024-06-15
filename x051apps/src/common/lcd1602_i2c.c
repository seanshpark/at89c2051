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

#define PCF8574_LCD1604_RS 0b00000001 // LCD160x_RS (Register Select: Inst / Data)
#define PCF8574_LCD1604_RW 0b00000010 // LCD160x_RW (R or /W)
#define PCF8574_LCD1604_EN 0b00000100 // LCD160x_EN (Enable)
#define PCF8574_LCD1604_BL 0b00001000 // LCD160x_BL (BackLignt)

static uint8_t _i2c_idx = 0;

static __bit _back_light = 0;
static __bit _display = 0;
static __bit _cursor = 0;
static __bit _blink = 0;

static void send_4bits(uint8_t lcddata)
{
  lcddata &= ~PCF8574_LCD1604_BL;

  // this sends 4bits to DB4~DB7 of HD44780
  // lower 4bits are used for control.
  // to write, send bits + EN high, wait 2usec, drop EN low, wait delay
  // data will be written falling edge
  lcddata |= PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  i2c_write_byte(1, 0, lcddata, _i2c_idx);
  delay10us();

  lcddata &= ~PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  i2c_write_byte(0, 1, lcddata, _i2c_idx);
}

static void init_4bit(void)
{
  uint8_t lcddata;
  uint8_t data;

  // send RS=0, RW=0, DB7~DB4=0011 as 8bit 3 times
  data = LCD1602_CMD_FUNCSET | LCD1602_FUNCSET_8BIT;

  lcddata = data & 0xf0;
  send_4bits(lcddata);
  delay10ms();

  lcddata = data & 0xf0;
  send_4bits(lcddata);
  delay200us();

  lcddata = data & 0xf0;
  send_4bits(lcddata);
  delay200us();

  // send RS=0, RW=0, DB7~DB4=0010 as 4bit 1 time
  data = LCD1602_CMD_FUNCSET;
  lcddata = data & 0xf0;
  send_4bits(lcddata);
  delay200us();
}

static void lcd1602_i2c_send_cmd(uint8_t data)
{
  uint8_t lcddata;

  // RS low is to select CONTROL
  // bit 7~4
  lcddata = (data & 0xf0);
  send_4bits(lcddata);
  delay10us();

  // bit 3~0
  lcddata = ((data & 0x0f) << 4);
  send_4bits(lcddata);
  delay10us();
}

static void lcd1602_i2c_send_data(uint8_t data)
{
  uint8_t lcddata;

  // RS high is to select DATA
  // bit 7~4
  lcddata = (data & 0xf0) | PCF8574_LCD1604_RS;
  send_4bits(lcddata);
  delay10us();

  // bit 3~0
  lcddata = ((data & 0x0f) << 4) | PCF8574_LCD1604_RS;
  send_4bits(lcddata);
  delay10us();
}

void lcd1602_i2c_init(uint8_t idx, uint8_t addr)
{
  _i2c_idx = idx;

  i2c_init(idx, addr);

  _display = 1;
  _cursor = 1;
  _blink = 1;
  _back_light = 1;

  init_4bit();
  delay100us();
  delay100us();
}

void lcd1602_i2c_display(__bit on, __bit cursor, __bit blink)
{
  uint8_t cmd;
  cmd = LCD1602_CMD_DISPLAY;
  cmd = cmd | (on ? LCD1602_DISPLAY_ON : 0);
  cmd = cmd | (cursor ? LCD1602_DISPLAY_CUR : 0);
  cmd = cmd | (blink ? LCD1602_DISPLAY_BLINK : 0);
  lcd1602_i2c_send_cmd(cmd);
  delay10ms();
}

void lcd1602_i2c_clear(void)
{
  uint8_t cmd = LCD1602_CMD_CLEAR;
  lcd1602_i2c_send_cmd(cmd);
  delay10ms();
}

void lcd1602_i2c_putch(uint8_t ch)
{
  lcd1602_i2c_send_data(ch);
  delay100us();
}

void lcd1602_i2c_puts(char *str)
{
  while (*str)
  {
    uint8_t ch = (uint8_t)(*str);
    lcd1602_i2c_putch(ch);
    str++;
  }
}

void lcd1602_i2c_move(uint8_t row, uint8_t col)
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
  lcd1602_i2c_send_cmd(cmd);
  delay100us();
}
