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

#include <sstream>
#include <iostream>

void arguments(arser::Arser &arser)
{
  arser.add_argument("--read", "-r")
    .nargs(1)
    .help("Read from AT89Cx051 and save to argument file");

  arser.add_argument("--erase", "-e")
    .nargs(0)
    .help("Erase AT89Cx051");

  arser.add_argument("--write", "-w")
    .nargs(1)
    .help("Write to AT89Cx051 from argument file");

  arser.add_argument("--i2c")
    .nargs(1)
    .help("I2C port in HEX, default is 0x20");
}

int main(int argc, char* argv[])
{
  arser::Arser arser;

  arguments(arser);

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

  int opt_cnt = 0;
  if (arser["--read"]) opt_cnt++;
  if (arser["--erase"]) opt_cnt++;
  if (arser["--write"]) opt_cnt++;
  if (opt_cnt != 1)
  {
    std::cout << "Error: Run with only one of read/erase/write options." << std::endl;
    std::cout << std::endl;
    std::cout << arser;
    return 1;
  }

  uint8_t i2c_address = 0x20;

  if (arser["--i2c"])
  {
    std::string addr = arser.get<std::string>("--i2c");
    i2c_address = std::stoul(addr, nullptr, 16);
  }
  std::cout << "I2C addr: 0x" << std::hex << (unsigned)i2c_address
            << " (dec " << std::dec << (unsigned)i2c_address << ")"
            << std::endl;

  if (arser["--read"])
  {
    std::string save_to_bin = arser.get<std::string>("--read");
    std::cout << "Read x051 and save to " << save_to_bin << std::endl;
  }

  if (arser["--write"])
  {
    std::string write_bin = arser.get<std::string>("--write");
    std::cout << "Write " << write_bin << " file to x051" << std::endl;
  }

  return 0;
}
