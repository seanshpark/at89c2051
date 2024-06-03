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

#include "mcp23017/mcp23017.h"

#include <iostream>
#include <bitset>
#include <cassert>

#define MCP23017_IODIRA   0x00  // A direction
#define MCP23017_IODIRB   0x01  // B direction

#define MCP23017_GPIOA    0x12  // A output
#define MCP23017_GPIOB    0x13  // B output
#define MCP23017_OLATA    0x14  // A output latch
#define MCP23017_OLATB    0x15  // B output latch

#define CHECK_PTR(PTR) \
  if (PTR == nullptr)  \
  {                    \
    assert(false);     \
    return;            \
  }

#define CHECK_PTR_0(PTR) \
  if (PTR == nullptr)    \
  {                      \
    assert(false);       \
    return 0;            \
  }

namespace rpigpio2
{

namespace
{

#pragma pack (1)
struct mcp23017Send
{
  uint8_t addr; // register address
  uint8_t data;
};
#pragma pack (0)

}

bool MCP23017::init(I2C *i2c)
{
  std::cout << "MCP23017::init" << std::endl;

  _i2c = i2c;

  send(MCP23017_IODIRA, 0x00); // port A to write mode
  send(MCP23017_IODIRB, 0x00); // port B to write mode
  send(MCP23017_GPIOA, 0xff);  // port A to high
  send(MCP23017_GPIOB, 0xff);  // port B to high

  _initalized = true;

  return true;
}

void MCP23017::release(void)
{
  CHECK_PTR(_i2c);

  send(MCP23017_GPIOA, 0xff);  // port A to high
  send(MCP23017_GPIOB, 0xff);  // port B to high
  _initalized = false;
  _i2c = nullptr;

  std::cout << "MCP23017::release" << std::endl;
}

// bit set -> read mode
// bit clr -> write mode
void MCP23017::modeA(uint8_t iomode)
{
  send(MCP23017_IODIRA, iomode);
}

void MCP23017::modeB(uint8_t iomode)
{
  send(MCP23017_IODIRB, iomode);
}

// Low 8bit is for PortA, High 8bit is for PortB
void MCP23017::write(uint16_t data)
{
  CHECK_PTR(_i2c);

  uint8_t dataa = (uint8_t)(data & 0xff);
  uint8_t datab = (uint8_t)((data >> 8) & 0xff);
  send(MCP23017_GPIOA, dataa);
  send(MCP23017_GPIOB, datab);
}

void MCP23017::writeA(uint8_t data)
{
  CHECK_PTR(_i2c);

  send(MCP23017_GPIOA, data);
}

void MCP23017::writeB(uint8_t data)
{
  CHECK_PTR(_i2c);

  send(MCP23017_GPIOB, data);
}

uint8_t MCP23017::readA(void)
{
  CHECK_PTR_0(_i2c);

  return read(MCP23017_GPIOA);
}

uint8_t MCP23017::readB(void)
{
  CHECK_PTR_0(_i2c);

  return read(MCP23017_GPIOB);
}

} // namespace rpigpio2

//
// MCP23017 privates
//
namespace rpigpio2
{

void MCP23017::send(uint8_t addr, uint8_t data)
{
  mcp23017Send mcpsend;
  mcpsend.addr = addr;
  mcpsend.data = data;
  // std::cout << "Addr:" << (int)addr << " Data:" << (int)data << std::endl;
  _i2c->write_buffer((uint8_t *)&mcpsend, 2);
}

uint8_t MCP23017::read(uint8_t addr)
{
  _i2c->write_byte(addr);

  uint8_t data;
  if (_i2c->read_byte(data))
    return data;
  return 0;
}

} // namespace rpigpio2
