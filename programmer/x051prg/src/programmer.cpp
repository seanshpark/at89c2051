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

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iterator>
#include <vector>

#include <unistd.h>

namespace x051prg
{

// MCP23017 GPIO port B bit and values
const uint8_t GPB0 = 0b00000001;
const uint8_t GPB1 = 0b00000010;
const uint8_t GPB2 = 0b00000100;
const uint8_t GPB3 = 0b00001000;
const uint8_t GPB4 = 0b00010000;
const uint8_t GPB5 = 0b00100000;
const uint8_t GPB6 = 0b01000000;
const uint8_t GPB7 = 0b10000000;

// x051 Vpp voltage control with GPBx
// - these values are from circuit design
const uint8_t RST_VPP_0V = GPB1 | GPB0; // H H
const uint8_t RST_VPP_5V = GPB1;        // H L
const uint8_t RST_VPP_12V = 0;          // L L

// x051 mode connction to MCP23017 GPB
const uint8_t P3_2 = GPB2;
const uint8_t P3_3 = GPB3;
const uint8_t P3_4 = GPB4;
const uint8_t P3_5 = GPB5;
const uint8_t P3_7 = GPB7;
const uint8_t XTAL1 = GPB6;

// READ MODE
// - RST/VPP to 5V
// - P3.2 H, P3.3 L, P3.4 L, P3.5 H, P3.7 H
// - PEROM Address is set to 0x0000 on rising edge of RST
// - Address is incremented at positive pulse of XTAL1
const uint8_t READ_MODE = RST_VPP_5V | P3_2 | P3_5 | P3_7;
const uint8_t POWER_OFF = RST_VPP_0V;

// WRITE MODE
// - RST/VPP to 12V
// - P3.2 H->L->H, P3.3 L, P3.4 H, P3.5 H, P3.7 H
// - PEROM Address is set to 0x0000 on rising edge of RST
// - Address is incremented at positive pulse of XTAL1
const uint8_t WRITE_MODE_5V = RST_VPP_5V | P3_2 | P3_4 | P3_5 | P3_7;
const uint8_t WRITE_MODE_12V = RST_VPP_12V | P3_2 | P3_4 | P3_5 | P3_7;

// ERASE MODE
// - RST/VPP to 12V
// - P3.2 H->L->H for 10ms
// - P3.3 H, P3.4 L, P3.5 L, P3.7 L
// - PEROM Address is set to 0x0000 on rising edge of RST
const uint8_t ERASE_MODE = RST_VPP_5V | P3_2 | P3_3;
const uint8_t ERASE_MODE_12V = RST_VPP_12V | P3_2 | P3_3;

namespace
{

void msleep(unsigned long msec)
{
  unsigned long usec = msec * 1000;
  usleep(usec);
}

} // namespace

void Programmer::run(void)
{
  std::cout << "x051prg, use /dev/i2c-" << _i2c_dev << ", address " << _i2c_addr << std::endl;

  _i2c = new rpigpio2::I2C();
  _i2c->init(nullptr, _i2c_dev, _i2c_addr);

  _mcp23017 = new rpigpio2::MCP23017();
  _mcp23017->init(_i2c);

  _mcp23017->modeA(0x00); // GPA as input mode
  _mcp23017->modeB(0x00); // GPB as input mode
  msleep(10);

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

  _mcp23017->release();
  _i2c->release();

  delete _mcp23017;
  delete _i2c;

  _mcp23017 = nullptr;
  _i2c = nullptr;
}

void Programmer::inc_address(uint8_t gpb_data)
{
  // increment address with 1 pulse to xtal-1
  gpb_data |= XTAL1;
  _mcp23017->writeB(gpb_data);
  usleep(1000);

  gpb_data &= ~XTAL1;
  _mcp23017->writeB(gpb_data);
  usleep(1000);
}

void Programmer::save_bin(std::vector<uint8_t> &buffer)
{
  // remove ending 0xff
  while (buffer.size())
  {
    size_t leng = buffer.size();
    if (buffer[leng - 1] != 0xff)
      break;
    buffer.pop_back();
  }

  std::ofstream output_file(_action_file, std::fstream::binary);
  std::ostream_iterator<uint8_t> output_it(output_file);
  std::copy(buffer.begin(), buffer.end(), output_it);
}

bool Programmer::load_bin(std::vector<uint8_t> &buffer)
{
  std::ifstream input_file(_action_file, std::fstream::binary);
  if (!input_file.good())
  {
    std::cout << "Failed to load: " << _action_file << std::endl;
    return false;
  }
  input_file.unsetf(std::ios::skipws);

  input_file.seekg(0, std::ios::end);
  size_t length = input_file.tellg();
  input_file.seekg(0, std::ios::beg);

  if (length > (size_t)_x051_size)
  {
    std::cout << "File is larger than PEROM size: " << length << " vs " << _x051_size << std::endl;
    return false;
  }

  buffer.resize(length);
  input_file.read((char *)&buffer[0], length);

  std::cout << "Dump: " << _action_file << std::endl;

  uint32_t addr = 0;
  uint32_t a16 = 0;
  for (const uint8_t &d : buffer)
  {
    if ((addr % 16) == 0)
      printf("%04x ", addr);
    printf("%02x ", d);
    if ((addr % 16) == 15)
      printf("\n");
    addr++;
  }
  printf("\n\n");

  return true;
}

// READ Sequence
// - RST to 5V
// - ADDR as 0x0000
// - READ from GPA
// - Pulse to XTAL1 to ADDR++
// - Loop until ADDR 2048
void Programmer::act_read(void)
{
  uint8_t gpa_data = 0x00;
  uint8_t gpb_data = 0x00;

  _mcp23017->modeA(0xff); // GPA as input mode
  _mcp23017->modeB(0x00); // GPA as output mode

  // reset to be sure with address be 0x0000
  gpb_data = 0x00;
  _mcp23017->writeB(gpb_data);
  msleep(5);

  // set read mode
  gpb_data = READ_MODE;
  _mcp23017->writeB(gpb_data);
  msleep(5);

  int32_t read_addr = 0;
  int32_t read_addr_p = read_addr;
  std::vector<uint8_t> buffer;
  std::vector<uint8_t> dump;

  while (read_addr < _x051_size)
  {
    gpa_data = _mcp23017->readA();
    buffer.push_back(gpa_data);
    dump.push_back(gpa_data);

    inc_address(gpb_data);
    read_addr++;

    if ((read_addr % 16) == 0)
    {
      printf("%04x ", read_addr_p);
      for (const uint8_t &d : dump)
        printf("%02x ", d);
      printf("\n");
      dump.clear();
      read_addr_p = read_addr;
    }
  }

  gpb_data = POWER_OFF;
  _mcp23017->writeB(gpb_data);

  save_bin(buffer);
}

// ERASE Sequence
// - RST to 5V
// - Set erase mode
// - RST to 12V
// - P3.2: H->L->H for 10msec
void Programmer::act_erase(void)
{
  uint8_t gpb_data = 0x00;

  // reset GPB
  gpb_data = 0x00;
  _mcp23017->writeB(gpb_data);
  msleep(5);

  // set erase mode
  gpb_data = ERASE_MODE;
  _mcp23017->writeB(gpb_data);
  msleep(5);

  // Vpp to 12V
  gpb_data = ERASE_MODE_12V;
  _mcp23017->writeB(gpb_data);
  msleep(1);

  // pulse low P3.2 for 10ms
  gpb_data = gpb_data & ~P3_2;
  _mcp23017->writeB(gpb_data);
  msleep(15);
  gpb_data = gpb_data | P3_2;
  _mcp23017->writeB(gpb_data);

  // P3.1 will be L (BUSY) while erase
  // just wait 1000msec for now
  msleep(1000);

  gpb_data = POWER_OFF;
  _mcp23017->writeB(gpb_data);
}

// WRITE Sequence
// - RST to 12V
// - ADDR as 0x0000
// - WRITE data to GPA
// - Pulse to XTAL1 to ADDR++
// - Loop until ADDR to length and program length
void Programmer::act_write(void)
{
  std::vector<uint8_t> buffer;
  if (!load_bin(buffer))
    return;

  uint8_t gpa_data = 0x00;
  uint8_t gpa_veri = 0x00;
  uint8_t gpb_data = 0x00;

  // reset to be sure with address be 0x0000
  gpb_data = 0x00;
  _mcp23017->writeB(gpb_data);
  msleep(5);

  int32_t write_addr = 0;
  int32_t write_size = buffer.size();
  while (write_addr < write_size)
  {
    if ((write_addr % 16) == 0)
    {
      printf("Program %04x ", write_addr);
      for (int32_t dd = 0; dd < 16 && write_addr + dd < write_size; dd++)
      {
        gpa_veri = buffer[write_addr + dd];
        printf("%02x ", gpa_veri);
      }
      printf("\n");
    }

    // Vpp to 5V, write ready
    gpb_data = WRITE_MODE_5V;
    _mcp23017->writeB(gpb_data);
    usleep(100);

    // data from file
    gpa_data = buffer[write_addr];
    _mcp23017->writeA(gpa_data);
    usleep(100);

    // Vpp to 12V, write mode
    gpb_data = WRITE_MODE_12V;
    _mcp23017->writeB(gpb_data);
    msleep(10);

    // pulse P3_2
    gpb_data = gpb_data & ~P3_2;
    _mcp23017->writeB(gpb_data);
    msleep(1);
    gpb_data = gpb_data | P3_2;
    _mcp23017->writeB(gpb_data);
    msleep(10); // be the Doc, typically takes 1.2ms

    // Vpp to 5V, read mode
    gpb_data = READ_MODE;
    _mcp23017->writeB(gpb_data);
    usleep(100);

    // verify data
    _mcp23017->modeA(0xff); // GPA as read
    msleep(1);
    gpa_veri = _mcp23017->readA();
    if (gpa_veri != gpa_data)
    {
      printf("Program error at %04x: %02x %02x\n", write_addr, gpa_data, gpa_veri);
      write_addr = 0xffff;
      break;
    }
    _mcp23017->modeA(0x00); // GPA as write
    msleep(1);

    inc_address(gpb_data);
    write_addr++;
  }

  gpb_data = POWER_OFF;
  _mcp23017->writeB(gpb_data);
}

} // namespace x051prg
