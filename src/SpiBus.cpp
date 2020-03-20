#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
// #include <pthread.h>
#include <unistd.h>


#include "SpiBus.h"

namespace Devices
{

    using namespace std;


    SpiBus::SpiBus(string devName, int devCs):myDevName(devName), 
        myFileDescriptor(0), myDevCs(devCs)
    {
        while(spiDeviceOpen() == SPI_BUSY)
        {
            // The resource is being used. Wait before trying again
            usleep(100);
        }
        spiSlaveSelect(myDevCs);

        uint32 aMode = spiDeviceList[myDevCs].Mode;
        uint32 aSpeed = spiDeviceList[myDevCs].Speed;
        myMode = spiSetMode(aMode, aSpeed);

    }

    SpiBus::~SpiBus()
    {
        spiSlaveSelect(SPI_DEVICE_NONE);
        spiSetMode(myMode, 0);
        spiDeviceClose();
    }

    uint32 SpiBus::spiDeviceOpen()
    {
        lock_guard<mutex> guard(myMutex);

        if(myFileDescriptor == 0)
        {
           myDevName = "/dev/"+myDevName; 
            myFileDescriptor = open (myDevName.c_str(), O_RDWR);
            if(myFileDescriptor < 0)
            {
                cout << "Error: open mbcpld spi failed ret =" << myFileDescriptor << "\n";
                myFileDescriptor = 0;
                return SPI_BUSY;
            }
        }
        return SPI_OK;
    }

    uint32 SpiBus::spiDeviceClose()
    {
        lock_guard<mutex> guard(myMutex);
        close(myFileDescriptor);
        myDevCs = 0;
        return SPI_OK;
    }

    void SpiBus::spiSlaveSelect(int spiDevice)
    {
        lock_guard<mutex> guard(myMutex);

        spiSlaveSelArgType *ioPtr = static_cast<spiSlaveSelArgType *>(malloc(sizeof(spiSlaveSelArgType)));
        if (NULL != ioPtr)
        {
            ioPtr->spiDevice = spiDevice;

            if(ioctl(myFileDescriptor, SPI_IOCTL_SET_SPISLAVE, ioPtr) != 0)
            {
                cout << "SpiSlaveSelector::spiSlaveSelect(" << spiDevice << ") failed\n";
            }
            free(ioPtr);
        } else
        {
            cout << "####!!!!!!!!!!!SpiSlaveSelector::spiSlaveSelect malloc failed\n";
        }
   
    }

    uint32 SpiBus::spiSetMode(uint32 theMode, uint32 spiFreq)
    {
        lock_guard<mutex> guard(myMutex);

        spiSetModeArgType *ioPtr = static_cast<spiSetModeArgType *>(malloc(sizeof(spiSetModeArgType)));
        if (NULL != ioPtr)
        {        
            ioPtr->theMode = theMode;
            ioPtr->spiFreq = spiFreq;
            myMode = ioctl(myFileDescriptor, SPI_IOCTL_SET_SPI_MODE, ioPtr);
            free(ioPtr);
        } else 
        {
            cout << "####!!!!!!!!!!!SpiSlaveSelector::spiSetMode malloc failed\n"; 
        }

        return myMode;
    }

    void SpiBus::spiSetCS(bool value)
    {
        // lock_guard<mutex> guard(myMutex);

        const int cs = value ? SPI_CS_SELECT : SPI_CS_DESELECT;
        spiChipSelArgType *ioPtr = static_cast<spiChipSelArgType *>(malloc(sizeof(spiChipSelArgType)));

        if (NULL != ioPtr)
        {
            ioPtr->spiDevice = myDevCs;
            ioPtr->spiCs     = cs;
            if(ioctl(myFileDescriptor, SPI_IOCTL_SET_CHIPSEL, ioPtr) != 0)
            {
                cout << "SpiTransaction::spiSetCS(" << value << ") failed\n";
            }
            free(ioPtr);
        } else 
        {
            cout << "####!!!!!!!!!!!SpiSlaveSelector::spiSetCS malloc failed\n"; 
        }
    }

    uint32 SpiBus::spiIO(SPI_DATA *pa)
    {
        lock_guard<mutex> guard(myMutex);   

        spiDataArgType data;
     
        memcpy(data.txBuffPtr, pa->txBuffPtr, pa->txBuffLen);
        data.txBuffLen = pa->txBuffLen;
        data.rxBuffLen = pa->rxBuffLen;

        spiSetCS(1);
    
        //cout << "txBuffer" << data.txBuffLen << "rxBuffer" << pa->rxBuffLen << endl;
        // Read/write access using kernel module
        if(ioctl(myFileDescriptor, SPI_IOCTL_SPI_IO, &data) != 0)
        {
            cout << "SPI IO wrong " << myFileDescriptor << endl;
            return SPI_FAIL;
        }
        memcpy(pa->rxBuffPtr, data.rxBuffPtr, pa->rxBuffLen);

        spiSetCS(0);
        // uint i, len = ((data.txBuffLen > data.rxBuffLen) ? data.txBuffLen : data.rxBuffLen);
        // char msg[8] = {0};
        // char *dout = (char*)(data.txBuffPtr);
        // char *din = (char*)(data.rxBuffPtr);
    
        // cout << endl;
        // cout << "SPI TX:";
        // for (i = 0; i < len; i++)
        // {
        //     sprintf(&msg[0], " %02x", dout[i]);
        //     cout << msg;
        // }
        // cout << endl;
    
        // cout << "SPI RX:";
        // for (i = 0; i < len; i++)
        // {
        //     sprintf(&msg[0], " %02x", din[i]);
        //     cout << msg;
        // }
        // cout << endl;
     
        return SPI_OK;        

    }

    uint32 SpiBus::readWrite(uint8 *txBuf, int txBufLen, uint8 *rxBuf, int rxBufLen)
    {
        SPI_DATA data;
        data.txBuffPtr = txBuf;
        data.txBuffLen = txBufLen;
        data.rxBuffPtr = rxBuf;
        data.rxBuffLen = rxBufLen;

        return spiIO(&data);
    }

    uint32 SpiBus::write(uint8 *txBuf, int txBufLen)
    {
        
        uint8 aReadBuffer[MAXRXBUFFERLGTH];
        return readWrite(txBuf, txBufLen, aReadBuffer, txBufLen);
    }


} // namespace Devices
