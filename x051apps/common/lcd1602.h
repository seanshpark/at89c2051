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

#ifndef __X051_APPS_COMMON_LCD1602_H__
#define __X051_APPS_COMMON_LCD1602_H__

// clang-format off
#define LCD1602_RS    P3_5
#define LCD1602_RW    P3_4
#define LCD1602_E     P3_3
#define LCD1602_DB    P1

// LCD1602 commands
#define LCD1602_CMD_DDRAMADDR     0b10000000
#define LCD1602_CMD_CGRAMADDR     0b01000000
#define LCD1602_CMD_FUNCSET       0b00100000
#define LCD1602_CMD_CURSOR        0b00010000
#define LCD1602_CMD_DISPLAY       0b00001000
#define LCD1602_CMD_ENTRY         0b00000100
#define LCD1602_CMD_RETHOME       0b00000010
#define LCD1602_CMD_CLEAR         0b00000001

// LCD1602_CMD_FUNCSET
#define LCD1602_FUNCSET_8BIT      0b00010000
#define LCD1602_FUNCSET_4BIT      0b00000000
#define LCD1602_FUNCSET_2LINES    0b00001000
#define LCD1602_FUNCSET_1LINES    0b00000000
#define LCD1602_FUNCSET_5x11      0b00000100
#define LCD1602_FUNCSET_5x8       0b00000000

// LCD1602_CMD_CURSOR
#define LCD1602_CURSOR_SHIFT_DIS  0b00001000 // display shift
#define LCD1602_CURSOR_SHIFT_CUR  0b00000000 // cursor move
#define LCD1602_CURSOR_RIGHT      0b00000100 // shift to the right
#define LCD1602_CURSOR_LEFT       0b00000000 // shift to the left

// LCD1602_CMD_DISPLAY
#define LCD1602_DISPLAY_ON        0b00000100
#define LCD1602_DISPLAY_OFF       0b00000000
#define LCD1602_DISPLAY_CUR       0b00000010
#define LCD1602_DISPLAY_CUR_OFF   0b00000000
#define LCD1602_DISPLAY_BLINK     0b00000001
#define LCD1602_DISPLAY_BLINK_OFF 0b00000000

// LCD1602_CMD_ENTRY
#define LCD1602_ENTRY_INC         0b00000010 // increment vs decrement
#define LCD1602_ENTRY_SHIFT       0b00000001 // entire shift on vs off
// clang-format on

#endif // __X051_APPS_COMMON_LCD1602_H__
