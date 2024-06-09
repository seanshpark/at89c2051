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

// wait for 10msec with Timer0
// - clock 11.0592MHz
// - timer clock input = 11.0592 / 12 = 0.9216
// - for 1msec = 921.6 times
// - for 10msec = 9216 times
// - timer value = 65536 - 9216 = 56320 = 0xdc00
void delay10ms(void)
{
  TMOD = 0x01; // timer 0 mode 1
  TH0 = 0xdc;  // for 10msec
  TL0 = 0x00;
  TR0 = 1; // start
  while (TF0 == 0)
  { // check overflow
    // clang-format off
__asm
    nop
__endasm;
    // clang-format on
  }
  TR0 = 0; // stop
  TF0 = 0; // clear overflow
}

// delay in 10msec unit
void delay10(uint8_t tmsec)
{
  for (uint8_t i = 0; i < tmsec; ++i)
    delay10ms();
}
