#ifndef MEMORY_MAPPED_ACCESSIBLE_DEVICE_HH
#define MEMORY_MAPPED_ACCESSIBLE_DEVICE_HH
/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2001 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    MemoryMappedAccessibleDevice.hh
 *
 *      Declaration for MemoryMappedAccessibleDevice class.  
 *      This class provides low-level read/write access to the MemoryMapped device.
 *
 *      @b Note: this is a abstract base class (it contains a pure-virtual
 *      method) and cannot be directly instantiated. 
 *
 * Type:    C++ include
 *
 *//*
 *
 * NOTE:
 *      Comments in this file use Doxygen style formatting and Doxygen
 *      formatting tags. See the documentation at http://www.doxygen.org
 *      for more information about the style and use of formatting tags.
 * 
 *************************************END***************************************
 */

#include "ss_gdef.h"                    // Tellabs standard data types
#include "AccessibleDevice.h"           // Base class for an accesible device
                                        // that contains methods common to all
					// accessible devices.

// Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Name:    MemoryMappedAccessibleDevice
     *
     * Description:
     *//**
     *      Declaration for low-level driver for memory mapped device.  At this level,
     *      the driver only provides primitive read/write access to the various
     *      registers within the device.
     *
     *//*
     *************************************END***************************************
     */
    class MemoryMappedAccessibleDevice : public virtual AccessibleDevice
    {
    public:
        // constructor - construct device object at a base address in memory.
        //               Notice default argument of 0 to allow this method
        //               to function as a default constructor also.
        MemoryMappedAccessibleDevice(uint32 baseAddress = 0);

        // destructor  - should be virtual if any methods are
        virtual ~MemoryMappedAccessibleDevice();

        // method to allow base address to be specified (or respecified) after construction
        virtual void setBaseAddress(uint32 baseAddress);
        virtual uint32 getBaseAddress(void) const;

    protected:
        virtual uint8 read(const volatile uint8 &deviceReg)     const;
        virtual uint16 read(const volatile uint16 &deviceReg)   const;
        virtual uint32 read(const volatile uint32 &deviceReg)   const;

        virtual void write(volatile uint8 &deviceReg, uint8 data);
        virtual void write(volatile uint16 &deviceReg, uint16 data);
        virtual void write(volatile uint32 &deviceReg, uint32 data);

        // protected data
        void *myBaseAddress;            // base address of the device in memory

    }; //class MemoryMappedAccessibleDevice

} // namespace Devices

#endif // MEMORY_MAPPED_ACCESSIBLE_DEVICE_HH

