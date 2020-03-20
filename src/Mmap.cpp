#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fstream>
#include <string>

#include "Mmap.h"

using  std::string;

#define completePriorAccessesBeforeContinuing() \
asm("    eieio             # enforce in-order execution for I/O"); 


Mmap::Mmap(std::string devName, uint32 size):
devName(devName),
size(size),
baseAddr(loadModulesAndGetBaseAddress(devName,size))
{}
Mmap::~Mmap()
{}
uint32 Mmap::loadModulesAndGetBaseAddress(string devName, uint32 size)
{
    uint32 theBaseAddress;
    // Need to load the  Kernel module before mapping against it.
    // bool koLoaded = false;
    // string line;
    string myDevName = "/dev/" + devName;

    // // Check if the main  Kernel module is already loaded, 
    // ifstream myfile("/proc/modules", ifstream::in);
    // if(!myfile.fail())
    // {
    //     while(getline(myfile, line) && !koLoaded)
    //     {
    //         if(line.find(devName) != string::npos)
    //         {
    //             koLoaded = true;
    //         }
    //     }

    //     myfile.close();
    // }

    // if(!koLoaded)
    // {
    //     //need to export MODULES env.
    //     string loadCmd = "$MODULES/" + devName + "_load";
    //     int ret = system(loadCmd.c_str());
    //     assert(ret == 0);
    // }

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


uint8 Mmap::readAtOffset8(uint32 offset)
{
            uint8 data = *reinterpret_cast<const volatile uint8 *>(baseAddr + offset);
            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset));

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

        return data;
}

uint16 Mmap::readAtOffset16(uint32 offset)
{
            uint16 data = *reinterpret_cast<const volatile uint16 *>(baseAddr + offset);
            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset));

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

        return data;
}

uint32 Mmap::readAtOffset32(uint32 offset)
{    uint32 data = *reinterpret_cast<const volatile uint32 *>(baseAddr + offset);
            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset));

    completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

    return data;
}


void Mmap::writeAtOffset8(uint32 offset, uint8 value)
{
            *reinterpret_cast<volatile uint8 *>
            (baseAddr + offset) = value;

            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset)) = value;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
}

void Mmap::writeAtOffset16(uint32 offset, uint16 value)
{
            *reinterpret_cast<volatile uint16 *>
            (baseAddr + offset) = value;

            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset)) = value;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
}

void Mmap::writeAtOffset32(uint32 offset, uint32 value)
{
            *reinterpret_cast<volatile uint32 *>
            (baseAddr + offset) = value;

            // (reinterpret_cast<uint32>(baseAddr) + reinterpret_cast<uint32>(offset)) = value;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
}
