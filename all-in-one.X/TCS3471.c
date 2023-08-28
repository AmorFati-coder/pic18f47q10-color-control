#include "TCS3471.h"
#include "mcc_generated_files/mcc.h"


void _i2cWrite(uint8_t address,void* data,size_t len){
    i2c_writeNBytes(address, data, len);
}
void _i2cRead(uint8_t address,void* data,size_t len){
    i2c_readNBytes(address, data, len);
}

void TCS3471_Init(){
    __delay_ms(100);
    TCS3471_EnableInterrupt();
    printf("TCS3471 interrupt enable\n");
    //2.4-700
    TCS3471_SetIntegrationTime(700.0);
    printf("TCS3471 set integration time\n");
    //7400
    TCS3471_SetWaitTime(200);
    printf("TCS3471 set wait time\n");
    TCS3471_SetGain(TCS3471_GAIN_1X);
    printf("TCS3471 set gain\n");
    //65535
    TCS3471_InterruptHighThreshold(33678);
    printf("TCS3471 set interrupt high threshold\n");
    TCS3471_InterruptLowThreshold(33677);
    printf("TCS3471 set interrupt low threshold\n");
    TCS3471_InterruptPersistence(2);
    printf("TCS3471 interrupt persistence\n");
    TCS3471_ClearInterrupt();
    printf("TCS3471 clear interrupt\n");
    TCS3471_Enable();
    printf("TCS3471 enable\n");
}

bool TCS3471_Detect()
{
    if (_detected)
        return true;
    else
    {
        _i2cAddress = TCS3471_ADDRESS_1;
        uint8_t tmpaddr = read8(TCS3471_ID_REG);
        if (tmpaddr == 0x14 || tmpaddr == 0x1D)
        {
            _detected = true;
        }
        else
            {
                _i2cAddress = TCS3471_ADDRESS_2;
                tmpaddr = read8(TCS3471_ID_REG);
                if (tmpaddr == 0x14 || tmpaddr == 0x1D)
                    {
                        _detected = true;
                    }
                else
                    {
                        _i2cAddress = 0;
                    }
            }
    }
    return _detected;
}

bool TCS3471_Enable()
{
    TCS3471_Detect();
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_PON_BIT);
        __delay_ms(3);
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_AEN_BIT);
        return true;
    }
    else
        return false;
}

void TCS3471_Disable()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~(TCS3471_PON_BIT | TCS3471_AEN_BIT));
    }
}

void TCS3471_SetIntegrationTime(float integrationTime)
{
    if (_detected)
    {
        uint16_t aTime = (uint16_t)(integrationTime * 10);
        aTime = aTime / 24;
        if (aTime > 256)
            aTime = 256;
        aTime = aTime - 256;
        write8(TCS3471_ATIME_REG, (uint8_t)aTime);
    }
}

void TCS3471_SetWaitTime(float waitTime)
{
    if (_detected)
    {
        int32_t wTime = (int32_t)(waitTime * 10);
        if (wTime < 24)
        {
            write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~TCS3471_WEN_BIT);
            return;
        }
        else if (wTime > 6144)
        {
            write8(TCS3471_CONFIG_REG, TCS3471_WLONG_BIT);
            if (wTime > 73728)
                wTime = 73728;
            wTime = wTime / 288;
        }
        else
        {
            write8(TCS3471_CONFIG_REG, 0x00);
            wTime = wTime / 24;
        }
        wTime = 256 - wTime;
        write8(TCS3471_WTIME_REG, (uint8_t)wTime);
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_WEN_BIT);
    }
}

void TCS3471_SetGain(tcs3471Gain_t gain)
{
    if (_detected)
    {
        write8(TCS3471_CONTROL_REG, gain);
    }
}

void TCS3471_EnableInterrupt()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_AIEN_BIT);
    }
}

void TCS3471_DisableInterrupt()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~TCS3471_AIEN_BIT);
    }
}

void TCS3471_ClearInterrupt()
{
    if (_detected)
    {
        _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_SPECIAL_BIT | TCS3471_INTCLEAR_BIT;
        _i2cWrite(_i2cAddress,_i2cBuffer,1);
    }
}

void TCS3471_InterruptHighThreshold(uint16_t highThreshold)
{
    if (_detected)
    {
        write16(TCS3471_AIHTL_REG,highThreshold);
    }
}

void TCS3471_InterruptLowThreshold(uint16_t lowThreshold)
{
    if (_detected)
    {
        write16(TCS3471_AILTL_REG,lowThreshold);
    }
}

void TCS3471_InterruptPersistence(uint8_t persistence)
{
    if (_detected)
    {
        uint8_t valueToWrite = persistence;
        if (valueToWrite > 60)
            valueToWrite = 60;
        if (valueToWrite > 3)
            valueToWrite = valueToWrite / 5 + 3;
        write8(TCS3471_PERS_REG,valueToWrite & 0x0F);
    }
}

uint8_t TCS3471_GetChipID()
{
    TCS3471_Detect();
    if (_detected)
    {
        return read8(TCS3471_ID_REG);
    }
    else
        return 0;
}

bool TCS3471_RgbcValid()
{
    if (_detected)
    {
        return (read8(TCS3471_STATUS_REG) & TCS3471_AVALID_BIT) == TCS3471_AVALID_BIT;
    }
    else
        return false;
}

uint16_t TCS3471_ReadCData()
{
    if (_detected)
    {
        return read16(TCS3471_CDATA_REG);
    }
    return 0;
}


uint16_t TCS3471_ReadRData()
{
    if (_detected)
    {
        return read16(TCS3471_RDATA_REG);
    }
    return 0;
}

uint16_t TCS3471_ReadGData()
{
    if (_detected)
    {
        return read16(TCS3471_GDATA_REG);
    }
    return 0;
}

uint16_t TCS3471_ReadBData()
{
    if (_detected)
    {
        return read16(TCS3471_BDATA_REG);
    }
    return 0;
}

uint16_t TCS3471_ReadCDataH(){
    if (_detected)
    {
        return read16(TCS3471_CDATAH_REG);
    }
    return 0;
}
uint16_t TCS3471_ReadRDataH(){
    if (_detected)
    {
        return read16(TCS3471_RDATAH_REG);
    }
    return 0;
}
uint16_t TCS3471_ReadGDataH(){
    if (_detected)
    {
        return read16(TCS3471_GDATAH_REG);
    }
    return 0;
}
uint16_t TCS3471_ReadBDataH(){
    if (_detected)
    {
        return read16(TCS3471_BDATAH_REG);
    }
    return 0;
}

void write8(uint8_t reg, uint8_t val)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | reg;
    _i2cBuffer[1] = val;
    _i2cWrite(_i2cAddress,_i2cBuffer,2);
}

void write16(uint8_t reg, uint16_t val)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_AUTOINCR_BIT | reg;
    _i2cBuffer[1] = val & 0xFF;
    _i2cBuffer[2] = (val >> 8) & 0xFF;
    _i2cWrite(_i2cAddress,_i2cBuffer,3);
}

uint8_t read8(uint8_t reg)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | reg;
    _i2cWrite(_i2cAddress, _i2cBuffer,1);
    _i2cRead(_i2cAddress,_i2cBuffer, 1);
    return _i2cBuffer[0];
}

uint16_t read16(uint8_t reg)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_AUTOINCR_BIT | reg;
    _i2cWrite(_i2cAddress, _i2cBuffer,1);
    _i2cRead(_i2cAddress, _i2cBuffer, 2);
    uint16_t ret = _i2cBuffer[1] << 8;
	ret |= _i2cBuffer[0];
    return ret;
}