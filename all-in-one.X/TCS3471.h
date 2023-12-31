/*
  TCS3471.h - TCS3471 Color light-to-digital converter IC by TAOS/AMS library
  Copyright (c) 2012, 2013 Raivis Rengelis (raivis [at] rrkb.lv). All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TCS3471_h
#define TCS3471_h

#include "mcc_generated_files/drivers/i2c_simple_master.h"
#include <stdint.h>

// slave address
#define TCS3471_ADDRESS_1   0x29
#define TCS3471_ADDRESS_2   0x39

// registers
#define TCS3471_ENABLE_REG  0x00
#define TCS3471_ATIME_REG   0x01
#define TCS3471_WTIME_REG   0x03
#define TCS3471_AILTL_REG   0x04
#define TCS3471_AILTH_REG   0x05
#define TCS3471_AIHTL_REG   0x06
#define TCS3471_AIHTH_REG   0x07
#define TCS3471_PERS_REG    0x0C
#define TCS3471_CONFIG_REG  0x0D
#define TCS3471_CONTROL_REG 0x0F
#define TCS3471_ID_REG      0x12
#define TCS3471_STATUS_REG  0x13
#define TCS3471_CDATA_REG   0x14
#define TCS3471_CDATAH_REG  0x15
#define TCS3471_RDATA_REG   0x16
#define TCS3471_RDATAH_REG  0x17
#define TCS3471_GDATA_REG   0x18
#define TCS3471_GDATAH_REG  0x19
#define TCS3471_BDATA_REG   0x1A
#define TCS3471_BDATAH_REG  0x1B

// command register bits
#define TCS3471_COMMAND_BIT  0x80
#define TCS3471_AUTOINCR_BIT 0x20
#define TCS3471_SPECIAL_BIT  0x60
#define TCS3471_INTCLEAR_BIT 0x06

// enable register bits
#define TCS3471_AIEN_BIT     0x10
#define TCS3471_WEN_BIT      0x08
#define TCS3471_AEN_BIT      0x02
#define TCS3471_PON_BIT      0x01

// configuration register bits
#define TCS3471_WLONG_BIT    0x02

// control register bits
typedef enum
{
    TCS3471_GAIN_1X = 0x00,
    TCS3471_GAIN_4X = 0x01,
    TCS3471_GAIN_16X = 0x02,
    TCS3471_GAIN_60X = 0x03,
}
tcs3471Gain_t;

// ID register values
#define TCS3471_1_5_VALUE    0x14
#define TCS3471_3_7_VALUE    0x1D

// status register bits
#define TCS3471_AINT_BIT     0x10
#define TCS3471_AVALID_BIT   0x01

uint8_t _i2cBuffer[3];
int _detected;
uint8_t _i2cAddress;
void _i2cWrite(uint8_t address,void* data,size_t len);
void _i2cRead(uint8_t address,void* data,size_t len);
void TCS3471_Init();

bool TCS3471_Detect();
bool TCS3471_Enable();
void TCS3471_disable();
void TCS3471_SetIntegrationTime(float integrationTime);
void TCS3471_SetWaitTime(float waitTime);
void TCS3471_SetGain(tcs3471Gain_t gain);

void TCS3471_EnableInterrupt();
void TCS3471_DisableInterrupt();
void TCS3471_ClearInterrupt();
void TCS3471_InterruptHighThreshold(uint16_t highThreshold);
void TCS3471_InterruptLowThreshold(uint16_t lowThreshold);
void TCS3471_InterruptPersistence(uint8_t persistence);
uint8_t TCS3471_GetChipID();
bool TCS3471_RgbcValid();
uint16_t TCS3471_ReadCData();
uint16_t TCS3471_ReadRData();
uint16_t TCS3471_ReadGData();
uint16_t TCS3471_ReadBData();

void write8(uint8_t reg, uint8_t val);
void write16(uint8_t reg, uint16_t val);
uint8_t read8(uint8_t reg);
uint16_t read16(uint8_t reg);



#endif