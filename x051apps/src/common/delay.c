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

// wait for about 10us with a loop
// assume clock is 11.0592MHz
// 1 cycle is 1/12 of clock
// 1 cycle is about 1 usec
// - mov : 1 cycle
// - djnz: 2 cycles
// - ret : 2 cycles
// - total = 1 + 2 * 4 + 2 = 11
void delay10us(void)
{
  // clang-format off
__asm
  mov   r7, #4
delay10us_repeat:
  djnz  r7, delay10us_repeat
__endasm;
  // clang-format on
}

// - total = 1 + 2 * 24 + 2 = 51
void delay50us(void)
{
  // clang-format off
__asm
  mov   r7, #24
delay50us_repeat:
  djnz  r7, delay50us_repeat
__endasm;
  // clang-format on
}

// - total = 1 + 2 * 49 + 2 = 101
void delay100us(void)
{
  // clang-format off
__asm
  mov   r7, #49
delay100us_repeat:
  djnz  r7, delay100us_repeat
__endasm;
  // clang-format on
}

// - total = 1 + 2 * 99 + 2 = 201
void delay200us(void)
{
  // clang-format off
__asm
  mov   r7, #99
delay200us_repeat:
  djnz  r7, delay200us_repeat
__endasm;
  // clang-format on
}

// - total = 1 + 2 * 249 + 2 = 501
void delay500us(void)
{
  // clang-format off
__asm
  mov   r7, #249
delay500us_repeat:
  djnz  r7, delay500us_repeat
__endasm;
  // clang-format on
}

void delay10ms(void)
{
  for (uint8_t i = 20; i > 0; i--)
    delay500us();
}

// delay in 10msec unit
void delay10(uint8_t tmsec)
{
  for (uint8_t i = 0; i < tmsec; ++i)
    delay10ms();
}
