#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fstream>

#include "MmapDevice.h"

namespace Devices
{
using namespace std;

MmapDevice::MmapDevice(string devName, uint32 size)
:MemoryMappedAccessibleDevice(loadModulesAndGetBaseAddress(devName, size)),
devName_(devName),
size_(size)
{}
MmapDevice::~MmapDevice()
{}
uint32 MmapDevice::loadModulesAndGetBaseAddress(string devName, uint32 size)
{
    uint32 theBaseAddress;
    // Need to load the  Kernel module before mapping against it.
    bool koLoaded = false;
    string line;
    string myDevName = "/dev/" + devName;

    // Check if the main  Kernel module is already loaded, 
    ifstream myfile("/proc/modules", ifstream::in);
    if(!myfile.fail())
    {
        while(getline(myfile, line) && !koLoaded)
        {
            if(line.find(devName) != string::npos)
            {
                koLoaded = true;
            }
        }

        myfile.close();
    }

    if(!koLoaded)
    {
        //need to export MODULES env.
        string loadCmd = "$MODULES/" + devName + "_load";
        int ret = system(loadCmd.c_str());
        assert(ret == 0);
    }

    // Open device so mmap() can be done against it
    int fd = open(myDevName.c_str(), O_RDWR);

    assert(fd > 0);

    void* mapAddr;
    mapAddr = mmap(NULL,                   // requested address to map to
                   size,    // size of range to map
                   PROT_READ | PROT_WRITE, // set both read and write permissions
                   MAP_SHARED,             // changes to range are shared
                   fd,                     // file descriptor for this device
                   0);

    // The device can close now
    close(fd);

    assert(mapAddr != MAP_FAILED);
    theBaseAddress = reinterpret_cast<uint32>(mapAddr);
    printf("Mmap driver initialized: Virtual Base Addr = 0x%08lX, Size = 0x%08X\n", theBaseAddress, size);
    return theBaseAddress;
}

}