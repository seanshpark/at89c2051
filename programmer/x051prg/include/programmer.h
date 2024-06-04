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

#include <string>

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
  void i2c_addr(unsigned long addr) { _i2c_address = static_cast<uint8_t>(addr); };
  unsigned long i2c_addr(void) const { return static_cast<unsigned long>(_i2c_address); }

  void rom_size(uint32_t size) { _x051_size = size; }

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

private:
  uint8_t _i2c_address = 0x20;
  uint32_t _x051_size = 2048;
  Action _action;
  std::string _action_file;
};

} // namespace x051prg

#endif // __X051_PRG_PROGRAMMER_H__
