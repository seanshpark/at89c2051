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

#include "programmer.h"

#include <i2c/i2c.h>

namespace x051prg
{

void Programmer::run(void)
{
  switch (_action)
  {
    case Action::READ:
      act_read();
      break;
    case Action::ERASE:
      act_erase();
      break;
    case Action::WRITE:
      act_write();
      break;
  }
}

void Programmer::act_read(void)
{
  //
}

void Programmer::act_erase(void)
{
  //
}

void Programmer::act_write(void)
{
  //
}

} // namespace x051prg
