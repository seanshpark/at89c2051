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

typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;
typedef __bit uint1_t;

//
// delay.c
//
void delay10us(void);
void delay50us(void);
void delay100us(void);
void delay200us(void);
void delay500us(void);
void delay10ms(void);
void delay10(uint8_t tmsec);

//
// lcd1602_direct.c
//
void lcd1602_send_cmd(uint8_t cmd);
void lcd1602_send_data(uint8_t data);
void lcd1602_init(void);
void lcd1602_display(__bit on, __bit cursor, __bit blink);
void lcd1602_clear(void);
void lcd1602_putch(uint8_t ch);
void lcd1602_puts(char *str);
void lcd1602_move(uint8_t row, uint8_t col);

//
// i2c.c
//
void i2c_init(uint8_t idx, uint8_t addr);
uint8_t i2c_write_byte(__bit send_start, __bit send_stop, uint8_t data, uint8_t idx);

//
// mcp23017.c
//
void mcp23017_init(uint8_t idx, uint8_t addr);
void mcp23017_modeA(uint8_t iomode);
void mcp23017_modeB(uint8_t iomode);
void mcp23017_writeA(uint8_t data);
void mcp23017_writeB(uint8_t data);

//
// lcd1602_i2c.c
//
void lcd1602_i2c_init(uint8_t idx, uint8_t addr);
void lcd1602_i2c_display(__bit on, __bit cursor, __bit blink);
void lcd1602_i2c_clear(void);
void lcd1602_i2c_putch(uint8_t ch);
void lcd1602_i2c_puts(char *str);
void lcd1602_i2c_move(uint8_t row, uint8_t col);
