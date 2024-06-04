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

#include "arser.h"
#include "programmer.h"

#include <string>
#include <sstream>
#include <iostream>

void init_options(arser::Arser &arser)
{
  arser.add_argument("--read", "-r").nargs(1).help("Read from AT89Cx051 and save to argument file");
  arser.add_argument("--erase", "-e").nargs(0).help("Erase AT89Cx051");
  arser.add_argument("--write", "-w").nargs(1).help("Write to AT89Cx051 from argument file");
  arser.add_argument("--dev", "-d")
    .nargs(1)
    .type(arser::DataType::INT32)
    .default_value(1)
    .help("/dev/i2c-? device number, default is 1");
  arser.add_argument("--i2c")
    .nargs(1)
    .type(arser::DataType::STR)
    .default_value("0x20")
    .help("I2C port in HEX, default is 0x20");
  arser.add_argument("--4051", "-4").nargs(0).help("Set to 4051 (4K ROM), default is 2051");
}

int check_options(arser::Arser &arser)
{
  int opt_cnt = 0;
  if (arser["--read"])
    opt_cnt++;
  if (arser["--erase"])
    opt_cnt++;
  if (arser["--write"])
    opt_cnt++;
  if (opt_cnt != 1)
  {
    std::cout << "Error: Run with only one of read/erase/write options." << std::endl;
    std::cout << std::endl;
    std::cout << arser;
    return 1;
  }
  if (arser["--dev"])
  {
    int32_t dev = arser.get<int32_t>("--dev");
    if (dev > 9 || dev < 0)
    {
      std::cout << "Invalid dev number. Please use 1 ~ 9" << std::endl;
      return 1;
    }
  }

  if (arser["--i2c"])
  {
    std::string addr = arser.get<std::string>("--i2c");
    int32_t value = static_cast<int32_t>(std::stoul(addr, nullptr, 16));
    if (value < 0 || value > 0x7f)
    {
      std::cout << "Invalid I2C address. Please use 0x00 ~ 0x7f" << std::endl;
      return 1;
    }
  }

  return 0;
}

void set_options(x051prg::Programmer &programmer, arser::Arser &arser)
{
  if (arser["--i2c"])
  {
    std::string addr = arser.get<std::string>("--i2c");
    programmer.i2c_addr(static_cast<int32_t>(std::stoul(addr, nullptr, 16)));
  }
  std::cout << "I2C address: 0x" << std::hex << programmer.i2c_addr() << " (dec " << std::dec
            << programmer.i2c_addr() << ")" << std::endl;

  if (arser["--dev"])
  {
    int32_t dev = arser.get<int32_t>("--dev");
    programmer.i2c_dev(dev);
  }
  std::cout << "I2C device: /dev/i2c-" << std::hex << programmer.i2c_dev() << std::endl;

  if (arser["--4051"])
  {
    programmer.rom_size(4096);
    std::cout << "Target: 4051" << std::endl;
  }

  if (arser["--read"])
  {
    std::string save_to_bin = arser.get<std::string>("--read");
    std::cout << "Read x051 and save to " << save_to_bin << std::endl;
    programmer.action(x051prg::Programmer::READ, save_to_bin);
  }

  if (arser["--erase"])
  {
    std::cout << "Erase "
              << "x051" << std::endl;
    programmer.action(x051prg::Programmer::ERASE);
  }

  if (arser["--write"])
  {
    std::string write_bin = arser.get<std::string>("--write");
    std::cout << "Write " << write_bin << " file to x051" << std::endl;
    programmer.action(x051prg::Programmer::WRITE, write_bin);
  }
}

int main(int argc, char *argv[])
{
  arser::Arser arser;

  init_options(arser);

  try
  {
    arser.parse(argc, argv);
  }
  catch (const std::runtime_error &err)
  {
    std::cout << err.what() << std::endl;
    std::cout << arser;
    return 1;
  }

  int check = check_options(arser);
  if (check)
    return check;

  x051prg::Programmer programmer;

  set_options(programmer, arser);

  programmer.run();

  return 0;
}
