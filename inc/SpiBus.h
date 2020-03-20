#ifndef SPIBUS_H
#define SPIBUS_H

#include "ss_gdef.h"
#include "mbCpldSpi.h"

#include <string>
#include <mutex>

namespace Devices
{


#define SPI_BIT_LENGTH(x) (((x) - 1) << 20)
const uint32 SpiModeRev       = SPI_MASTER | SPI_BIT_LENGTH(8);
const uint32 SpiModeRevInv    = SPI_CLK_INVERT |  SPI_MASTER | SPI_BIT_LENGTH(8);
const uint32 SpiModeNormInv   = SPI_CLK_INVERT | SPI_CLK_PHASE | SPI_NORM_DATA | SPI_MASTER | SPI_BIT_LENGTH(8);
const uint32 SpiModeNormNoPh  = SPI_NORM_DATA | SPI_MASTER | SPI_BIT_LENGTH(8);
const uint32 SpiModeNorm      = SPI_MASTER | SPI_BIT_LENGTH(8) | SPI_NORM_DATA;
const uint32 SpiModeNormDiv16 = SPI_CLK_DIV16 | SPI_MASTER | SPI_BIT_LENGTH(8) | SPI_NORM_DATA;
const uint32 SpiModeRev12     = SPI_MASTER | SPI_BIT_LENGTH(12);
const uint32 SpiModeInv       = SPI_MASTER | SPI_CLK_INVERT | SPI_NORM_DATA | SPI_BIT_LENGTH(8);
const uint32 SpiModePhInv     = SPI_CLK_INVERT | SPI_CLK_PHASE | SPI_MASTER | SPI_BIT_LENGTH(8);
const uint32 SpiModeNormGX    = SPI_MASTER | SPI_NORM_DATA;

typedef struct
{
    uint32 Mode;
    uint32 Speed;            // In Hertz
    const char *Name;
} DeviceList;

// Important Note: This card has SPI hardware in the CPLD, and several ports exist.
static DeviceList spiDeviceList[] =
{
    {SpiModeNormInv,  3000000, "DIGI1 ARMBOOT SPI Flash"},
    {SpiModeNormInv,  3000000, "DIGI2 ARMBOOT SPI Flash"},
    {SpiModeNormInv,  3000000, "DIGI3 ARMBOOT SPI Flash"},
    {SpiModeNormNoPh, 3000000, "BCM5389 GE Switch"},
    {SpiModeNormNoPh, 3000000, "SFPPC AS CPLD"},
    {SpiModeNormNoPh, 3000000, "SFPPC PROM CPLD"},
    {SpiModeNormInv,  3000000, "DIGI1 SPI SLAVE CS0"},
    {SpiModeNormInv,  3000000, "DIGI1 SPI SLAVE CS1"},
    {SpiModeNormInv,  3000000, "DIGI2 SPI SLAVE CS0"},
    {SpiModeNormInv,  3000000, "DIGI2 SPI SLAVE CS1"},
    {SpiModeNormGX,   3000000, "DIGX SPI Switch"},
};

using namespace std;
class SpiBus
{   public:
        explicit SpiBus(string devName, int DevCs);
        ~SpiBus();

        uint32 spiDeviceOpen();
        uint32 spiDeviceClose();
        void spiSlaveSelect(int spiDevice);
        uint32 spiSetMode(uint32 theMode, uint32 spiFreq);

        void spiSetCS(bool value);
        uint32 spiIO(SPI_DATA *pa);
        uint32 readWrite(uint8 *txBuf, int txBufLen, uint8 *rxBuf, int rxBufLen);
        uint32 write(uint8 *txBuf, int txBufLen);
        

    private:
        string myDevName;
        int myFileDescriptor;
        int myDevCs;
        uint32 myMode;

        // This mutex is needed since we have a single SPI master in CPLD. The selection is done
        // by CPLD register and we cannot access more then one device at the same time.
        // static pthread_mutex_t mySpiDeviceMutex;
        mutex myMutex;
};

}
#endif