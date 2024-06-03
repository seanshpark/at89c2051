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

#ifndef __RPIGPIO2_LIB_MCP23017_H__
#define __RPIGPIO2_LIB_MCP23017_H__

#include "i2c/i2c.h"

namespace rpigpio2
{

class MCP23017
{
public:
  MCP23017() = default;
  virtual ~MCP23017() = default;

public:
  bool init(I2C *i2c);
  void release(void);

  void modeA(uint8_t iomode);
  void modeB(uint8_t iomode);

  void write(uint16_t data);
  void writeA(uint8_t data);
  void writeB(uint8_t data);

  uint8_t readA(void);
  uint8_t readB(void);

public:
  bool initialized(void) { return _initalized; }

private:
  void send(uint8_t addr, uint8_t data);
  uint8_t read(uint8_t addr);

private:
  I2C *_i2c = nullptr;
  bool _initalized = false;
};

} // namespace rpigpio2

#endif // __RPIGPIO2_LIB_MCP23017_H__
