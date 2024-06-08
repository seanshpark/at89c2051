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

void main()
{
  delay10(50);

  mcp23017_init(0x20);
  mcp23017_modeA(0b11111111); // A as read mode
  mcp23017_modeB(0b00000000); // B as write mode

  while (1)
  {
    BLINK_LED = 1;
    mcp23017_writeB(0b11111111);
    delay10(50);

    BLINK_LED = 0;
    mcp23017_writeB(0b00000000);
    delay10(50);
  }
}
