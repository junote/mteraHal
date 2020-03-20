#include "I2cDev.h"
#include <cstdio>
#include "BcmCli.h"



int main()
{
    Devices::I2cDev t("/dev/i2c-1", 0x9a);
    printf("read tmp441 0x%x\n", t.read8(0));


    Devices::BcmCli bcm("/mnt/sdcard/bcm/libBcm.so");
    bcm.start();



    return 0;
}