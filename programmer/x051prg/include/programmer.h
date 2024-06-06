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

#ifndef __X051_PRG_PROGRAMMER_H__
#define __X051_PRG_PROGRAMMER_H__

#include <i2c/i2c.h>
#include <mcp23017/mcp23017.h>

#include <iostream>
#include <string>
#include <vector>

namespace x051prg
{

class Programmer
{
public:
  Programmer() = default;

  enum Action
  {
    UNDEFINED = 0,
    READ,
    ERASE,
    WRITE
  };

public:
  void i2c_addr(int32_t addr) { _i2c_addr = addr; };
  int32_t i2c_addr(void) const { return _i2c_addr; }

  void i2c_dev(int32_t dev) { _i2c_dev = dev; };
  int32_t i2c_dev(void) const { return _i2c_dev; }

  void rom_size(int32_t size) { _x051_size = size; }

  void action(Action action, const std::string filename)
  {
    _action = action;
    _action_file = filename;
  }
  void action(Action action)
  {
    _action = action;
    _action_file.clear();
  }

  void run(void);

private:
  void act_read(void);
  void act_erase(void);
  void act_write(void);

  void inc_address(uint8_t gpb_data);
  void save_bin(std::vector<uint8_t> &buffer);
  bool load_bin(std::vector<uint8_t> &buffer);

private:
  int32_t _i2c_addr = 0x20;
  int32_t _i2c_dev = 1;
  int32_t _x051_size = 2048;
  Action _action;
  std::string _action_file;

  rpigpio2::I2C *_i2c = nullptr;
  rpigpio2::MCP23017 *_mcp23017 = nullptr;
};

} // namespace x051prg

#endif // __X051_PRG_PROGRAMMER_H__
