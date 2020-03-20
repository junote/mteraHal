#ifndef I2CBUS_H
#define I2CBUS_H



#include <string>
#include "ss_gdef.h"



namespace Devices
{

using namespace std;
class I2cBus
{   public:
        explicit I2cBus(string devName);
        ~I2cBus();

        bool i2cRead(uint8 *txBuf, int txBufLen, uint8 *rxBuf, int rxBufLen);
        bool i2cWrite(uint8* txBuf, int txBufLen);
        bool i2cRepeatStartRead(uint8* txBuf, int txBufLen, uint8* txBuf2,int txBufLen2, uint8* rxBuf, int rxBufLen);

    private:
        string devName_;
        int myFileDescriptor;
};

}
#endif