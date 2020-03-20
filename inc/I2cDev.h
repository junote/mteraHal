#ifndef I2CDEV_H
#define I2CDEV_H

#include <string>
#include "I2cBus.h"
#include "ss_gdef.h"

namespace Devices
{
    using namespace std;
    class I2cDev
    {
        public:
            explicit I2cDev(string devName_, uint8 devAddr_);
            ~I2cDev(){}

            uint8 read8(uint8 regAddr);
            bool write8(uint8 regAddr, uint8 value);

        private:
            string devName;
            uint8 devAddr;
            I2cBus myBus;
    };
}
#endif