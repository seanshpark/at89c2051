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

//
// delay.c
//
void delay10us(void);
void delay50us(void);
void delay100us(void);
void delay10ms(void);
void delay10(uint8_t tmsec);

//
// lcd1602.c
//
void lcd1602_send_cmd(uint8_t cmd);
void lcd1602_send_data(uint8_t data);
void lcd1602_init(void);
void lcd1602_display(int on, int cursor, int blink);
void lcd1602_clear(void);
void lcd1602_putch(uint8_t ch);
void lcd1602_puts(char *str);
void lcd1602_move(uint8_t row, uint8_t col);
