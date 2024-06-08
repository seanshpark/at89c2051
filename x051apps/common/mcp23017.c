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

#define MCP23017_IODIRA 0x00 // A direction
#define MCP23017_IODIRB 0x01 // B direction

#define MCP23017_GPIOA 0x12 // A output
#define MCP23017_GPIOB 0x13 // B output
#define MCP23017_OLATA 0x14 // A output latch
#define MCP23017_OLATB 0x15 // B output latch

static void _send(uint8_t addr, uint8_t data)
{
  i2c_write_byte(1, 0, addr);
  i2c_write_byte(0, 1, data);
}

void mcp23017_init(uint8_t addr)
{
  i2c_init(addr); //
}

void mcp23017_modeA(uint8_t iomode)
{
  _send(MCP23017_IODIRA, iomode); //
}

void mcp23017_modeB(uint8_t iomode)
{
  _send(MCP23017_IODIRB, iomode); //
}

void mcp23017_writeA(uint8_t data)
{
  _send(MCP23017_GPIOA, data); //
}

void mcp23017_writeB(uint8_t data)
{
  _send(MCP23017_GPIOB, data); //
}
