#include "I2cDev.h"
#include <iostream>
#include <cstdio>

namespace Devices
{
    using namespace std;

    I2cDev::I2cDev(string devName_, uint8 devAddr_):
        devName(devName_),
        devAddr(devAddr_),
        myBus(I2cBus(devName))
    {}
    uint8 I2cDev::read8(uint8 regAddr)
    {
        uint8 data = 0;
        uint8 dataBuf[sizeof(data) + 1];

        uint8 addrBuf[2];
        uint8 *ptr = &addrBuf[0];



        *ptr++ = (devAddr & (~0x1));
        *ptr++= regAddr;

        uint8 txBuf2 = devAddr | 0x1;

        // for(int i = 0; i < 2; ++i)
            // printf("addrBuf %d:0x%x\n",i,addrBuf[i]);
        // printf("txbuf2: 0x%x\n",txBuf2);

        bool ret = myBus.i2cRepeatStartRead(&addrBuf[0],sizeof(addrBuf),
                 &txBuf2, sizeof(txBuf2),&dataBuf[0],sizeof(dataBuf));
        if (!ret)
        { 
            printf("i2cDev read with %s devAddr 0x%x regAddr 0x%x\n",devName.c_str(),devAddr,regAddr);
            return -1;
        } else
        {
            // for(int i = 0; i < 2; ++i)
                // printf("dataBuf %d:0x%x\n",i,dataBuf[i]);
            ptr = &dataBuf[0];
            for (int i = 1; i <= sizeof(data); i++)
            {
                data |= (*ptr++ << ((sizeof(data) - i) * 8));
                // printf("data = 0x%x\n",data);
            }
            return data;
        }
    }

    bool I2cDev::write8(uint8 regAddr, uint8 value)
    {
        uint8 addrData[2 + sizeof(value)];
        uint8 *ptr = &addrData[0];
        *ptr++ = devAddr & (~0x1);
        *ptr++ = regAddr;
        *ptr++ = value;

        // for(int i = 0; i < 3; ++i)
        //     printf("addrBuf %d:0x%x\n",i,addrData[i]);

        bool ret = myBus.i2cWrite(&addrData[0], (ptr - &addrData[0]));

        return ret;

    }
    
} // namespace devices

