#ifndef MMAPDEVICE_H
#define MMAPDEVICE_H

#include "MemoryMappedAccessibleDevice.h"

#include <string>



namespace Devices
{

using namespace std;
class MmapDevice : public MemoryMappedAccessibleDevice
{
    public:
        explicit MmapDevice(string devName, uint32 size);
        virtual ~MmapDevice();


        virtual uint getInstanceNumber() const {return 0;}
        uint8 read8(uint32 offset){return static_cast<uint>(debugReadAtOffset8(offset));}
        void write8(uint32 offset, uint8 value){debugWriteAtOffset8(offset, value);}
        uint16 read16(uint32 offset){return static_cast<uint>(debugReadAtOffset16(offset));}
        void write16(uint32 offset, uint16 value){debugWriteAtOffset16(offset, value);}
        uint32 read32(uint32 offset){return static_cast<uint>(debugReadAtOffset32(offset));}
        void write32(uint32 offset, uint32 value){debugWriteAtOffset32(offset, value);}
    private:
        string devName_;
        uint32 size_;
        uint32 loadModulesAndGetBaseAddress(string devName, uint32 size);
    
};
}
#endif