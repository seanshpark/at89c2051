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

#define PIN_SCL P1_2
#define PIN_SDA P1_3

#define I2C_RETRY 255

static __bit _started;
static uint8_t _addr;

void i2c_init(uint8_t addr)
{
  PIN_SDA = 1;
  PIN_SCL = 1;

  _addr = addr;
  _started = 0;
}

static inline void _delay(void)
{
  //
  delay10us();
}

static void _dummy_clock(void)
{
  // do dummy clock pulse skip read ack
  PIN_SCL = 1;
  _delay();
  PIN_SCL = 0;
  _delay();
}

static void _wait_scl(void)
{
  uint8_t retry = I2C_RETRY;
  while (PIN_SCL == 0)
  {
    if (!retry)
      break;
    retry--;
    _delay();
  }
}

static void start_cond(void)
{
  if (_started)
  {
    PIN_SDA = 1;
    _delay();
    PIN_SCL = 1;

    _wait_scl();
    _delay();
  }
  _delay();

  PIN_SDA = 0;
  _delay();
  PIN_SCL = 0;
  _delay();
  _delay();

  _started = 1;
}

static void stop_cond(void)
{
  _delay();

  PIN_SCL = 1;
  _wait_scl();
  _delay();

  PIN_SDA = 1;
  _delay();

  _started = 0;
}

static void write_bit(__bit bit)
{
  PIN_SDA = bit;

  _delay();
  PIN_SCL = 1;
  _delay();
  PIN_SCL = 0;
  _delay();
}

/*
static __bit read_bit(void)
{
  __bit bit;

  PIN_SDA = 1;
  _delay();

  PIN_SCL = 1;
  // _wait_scl();
  _delay();
  bit = PIN_SDA;
  PIN_SCL = 0;
  _delay();

  return bit;
}
*/

uint8_t i2c_write_byte(__bit send_start, __bit send_stop, uint8_t data)
{
  uint8_t bit;
  uint8_t send;
  __bit nack = 0;

  if (send_start)
  {
    start_cond();
    _delay();

    // send 7bit addr, from MSB to LSB
    send = _addr;
    send <<= 1;
    for (bit = 7; bit > 0; --bit)
    {
      write_bit(send & 0x80 ? 1 : 0);
      send <<= 1;
    }
    // R/W
    // -  Read: master <-- slave : 1
    // - Write: master --> slave : 0
    write_bit(0);

    // read ack
    // nack = read_bit();
    // skip ack
    write_bit(0);
  }

  // send data
  send = data;
  for (bit = 8; bit > 0; --bit)
  {
    write_bit(send & 0x80 ? 1 : 0);
    send <<= 1;
  }

  // nack = read_bit();
  //  skip ack
  write_bit(0);

  if (send_stop)
    stop_cond();

  return nack;
}
