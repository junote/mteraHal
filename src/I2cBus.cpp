#include <string>
#include "ss_gdef.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#include <iostream>
#include <cstdio>
#include <cstring>

#include "I2cBus.h"

namespace Devices
{

using namespace std;

I2cBus::I2cBus(string devName):devName_(devName)
{
    if(!devName.empty())
    {  
        devName = "/dev/" + devName;
        myFileDescriptor = open(devName.c_str(), O_RDWR | O_SYNC);
        if(myFileDescriptor == -1)
        {
            cout << "* ERROR I2cBus: o/s device not open\n";
        }
    }    
}

I2cBus::~I2cBus()
{
    close(myFileDescriptor);
}

bool I2cBus::i2cRead(uint8 *txBuf, int txBufLen,
                        uint8 *rxBuf, int rxBufLen)
{
    bool rv = true;

    unsigned char slaveAddr = *(static_cast<unsigned char *>(txBuf));
    struct i2c_rdwr_ioctl_data i2c_work_queue = { 0, 0 };

    if(myFileDescriptor == -1)
    {
        cout << "* ERROR I2cBus: o/s device not open\n";
        return false;
    }

    // Clear the rx buffer
    memset(rxBuf, 0, rxBufLen);

    if(txBufLen > 1)
    {
        i2c_work_queue.nmsgs = 2;
    }
    else
    {
        i2c_work_queue.nmsgs = 1;
    }

    i2c_work_queue.msgs = reinterpret_cast<struct i2c_msg *>(malloc(i2c_work_queue.nmsgs * sizeof(struct i2c_msg)));
    if(!i2c_work_queue.msgs)
    {
        cout << "* ERROR I2cBus: malloc error for ioctl work queue\n";
        rv = false;
    }
    else
    {
        if(txBufLen > 1)
        {
            // configure write msg
            i2c_work_queue.msgs[0].addr = (static_cast<int>(slaveAddr >> 1));
            i2c_work_queue.msgs[0].flags = 0; // write
            i2c_work_queue.msgs[0].buf = (reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned long>(txBuf)) + 1);
            i2c_work_queue.msgs[0].len = txBufLen - 1;

            // configure read msg (i2c driver will automagically do rpt-start)
            i2c_work_queue.msgs[1].addr = (static_cast<int>(slaveAddr >> 1));
            i2c_work_queue.msgs[1].flags = I2C_M_RD; // read
            i2c_work_queue.msgs[1].buf = (static_cast<unsigned char *>(rxBuf));
            i2c_work_queue.msgs[1].len = rxBufLen;
        }
        else
        {
            // configure read msg
            i2c_work_queue.msgs[0].addr = (static_cast<int>(slaveAddr >> 1));
            i2c_work_queue.msgs[0].flags = I2C_M_RD; // read
            i2c_work_queue.msgs[0].buf = (static_cast<unsigned char *>(rxBuf));
            i2c_work_queue.msgs[0].len = rxBufLen;
        }

        if(ioctl(myFileDescriptor, I2C_RDWR, &i2c_work_queue) < 0)
        {
            printf( "* ERROR I2cBus: i2cRead ioctl RDWR error addr0:0x%x addr1: 0x%x \n\n",
                i2c_work_queue.msgs[0].addr, i2c_work_queue.msgs[1].addr);
            rv = false;
        }

        free(i2c_work_queue.msgs);
    }

    return rv;
}



bool I2cBus::i2cWrite(uint8* txBuf, int txBufLen)
{
    bool rv = true;

    unsigned char slaveAddr = *(static_cast<unsigned char *>(txBuf));
    struct i2c_rdwr_ioctl_data i2c_work_queue = { 0, 0 };

    if(myFileDescriptor == -1)
    {
        cout << "* ERROR I2cBus: o/s device not open\n\n";
        rv = false;
    }
    else
    {
        i2c_work_queue.nmsgs = 1;
        i2c_work_queue.msgs = reinterpret_cast<struct i2c_msg *>(malloc(i2c_work_queue.nmsgs * sizeof(struct i2c_msg)));
        if(!i2c_work_queue.msgs)
        {
            cout << "* ERROR I2cBus: malloc error for ioctl work queue\n";
            rv = false;
        }
        else
        {
            // configure write msg
            i2c_work_queue.msgs[0].addr = (static_cast<int>(slaveAddr >> 1));
            i2c_work_queue.msgs[0].flags = 0; // write
            i2c_work_queue.msgs[0].buf = (reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned long>(txBuf)) + 1);
            i2c_work_queue.msgs[0].len = txBufLen - 1;

            // printf("msg:%d,%d,%d,%d\n", i2c_work_queue.msgs[0].addr,i2c_work_queue.msgs[0].flags,*(i2c_work_queue.msgs[0].buf),i2c_work_queue.msgs[0].len);

            if(ioctl(myFileDescriptor, I2C_RDWR, &i2c_work_queue) < 0)
            {
                cout << "* ERROR I2cBus: i2cWrite ioctl RDWR error addr0:" << i2c_work_queue.msgs[0].addr << "\n\n";
                rv = false;
            }

            free(i2c_work_queue.msgs);
        }
    }

}

bool I2cBus::i2cRepeatStartRead(uint8* txBuf, int txBufLen,
                                uint8* txBuf2, int txBufLen2,
                                uint8* rxBuf, int rxBufLen)
{
    bool rv = true;

    struct i2c_rdwr_ioctl_data i2c_work_queue = { 0, 0 };

    if(myFileDescriptor == -1)
    {
        cout << "* ERROR I2cBus: o/s device not open\n";
        return false;
    }

    if((txBufLen < 1) || (txBufLen2 != 1))
    {
        // Assume that txBuf2 is always only 1 byte long, and contains
        // only the slave Address for the read transfer.
        cout << "* ERROR I2cBus: tx buf len must be >= 1\n";
        return false;
    }

    i2c_work_queue.nmsgs = 2;
    i2c_work_queue.msgs = reinterpret_cast<struct i2c_msg *>(malloc(i2c_work_queue.nmsgs * sizeof(struct i2c_msg)));
    if(!i2c_work_queue.msgs)
    {
        printf("malloc error for ioctl work queue\n");
        rv = false;
    }
    else
    {
        // Clear the rx buffer.
        memset(rxBuf, 0, rxBufLen);

        // configure write part
        unsigned char slaveAddr = *(static_cast<unsigned char *>(txBuf));
        i2c_work_queue.msgs[0].addr = (static_cast<int>(slaveAddr >> 1));
        i2c_work_queue.msgs[0].flags = 0; // write
        i2c_work_queue.msgs[0].buf = (reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned long>(txBuf)) + 1);
        i2c_work_queue.msgs[0].len = txBufLen - 1;

        // configure read part (i2c driver will automagically do rpt-start)
        slaveAddr = *(static_cast<unsigned char *>(txBuf2));
        i2c_work_queue.msgs[1].addr = (static_cast<int>(slaveAddr >> 1));
        i2c_work_queue.msgs[1].flags = I2C_M_RD; // read
        i2c_work_queue.msgs[1].buf = (static_cast<unsigned char *>(rxBuf));
        i2c_work_queue.msgs[1].len = rxBufLen;

        if(ioctl(myFileDescriptor, I2C_RDWR, &i2c_work_queue) < 0)
        {
            printf("* ERROR I2cBus: Device 0x%02X had ioctl RDWR error, errno=%d\n", slaveAddr, errno);
            rv = false;
        }

        free(i2c_work_queue.msgs);
    }

    return rv;
}
}
