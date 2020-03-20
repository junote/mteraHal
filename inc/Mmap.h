#ifndef MMAP_H
#define MMAP_H

#include <string>
#include <cstdint>

#include "ss_gdef.h"


class Mmap
{
    public:
        explicit Mmap(std::string devName, uint32 size);
        virtual ~Mmap();


        uint8 readAtOffset8(uint32 offset);
        void writeAtOffset8(uint32 offset, uint8 value);
        uint16 readAtOffset16(uint32 offset);
        void writeAtOffset16(uint32 offset, uint16 value);
        uint32 readAtOffset32(uint32 offset);
        void writeAtOffset32(uint32 offset, uint32 value);
    private:
        std::string devName;
        uint32 size;
        uint32 baseAddr;
        uint32 loadModulesAndGetBaseAddress(std::string devName, uint32 size);

    
};

#endif