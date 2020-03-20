/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2001 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    MemoryMappedAccessibleDevice.cc
 *
 *      Implementation of the MemoryMappedAccessibleDevice class
 *
 * Type:    C++ source
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

#include "ss_gdef.h"                        // Tellabs standard data types
#include "MemoryMappedAccessibleDevice.h"   // declaration for MemoryMappedAccessibleDevice 
                                            //   class
#include "PlatformDependent.h"              // For completePriorAccessesBeforeContinuing()
// #include "cCritSect.h"                      // Critical section class - used by
                                            //   the "atomic" functions below

// Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Function:    MemoryMappedAccessibleDevice
     *
     * Description:
     *//**
     *      Constructor for MemoryMappedAccessibleDevice class - establishes
     *      a memory mapped device at the specified base address
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b baseAddress - the base address of the memory-mapped Asic device
     *
     * @b Outputs:
     *
     *    Class Data:
     *      @arg @b myBaseAddress -   set to baseAddress argument for constructor
     *
     * @returns
     *      None
     *
     *//*
     *************************************END***************************************
     */
    MemoryMappedAccessibleDevice::MemoryMappedAccessibleDevice(uint32 baseAddress) :
    myBaseAddress(reinterpret_cast<void *>(baseAddress))
    {
    }

    /************************************BEGIN**************************************
     *
     * Function:    ~MemoryMappedAccessibleDevice
     *
     * Description:
     *//**
     *      Destructor for MemoryMappedAccessibleDevice class.  Currently 
     *      it doesn't do anything, but is provided for completeness.
     *
     * @b Inputs:
     *      @arg none
     *
     * @b Outputs:
     *      @arg none
     *
     * @returns
     *      None
     *
     *//*
     *************************************END***************************************
     */
    MemoryMappedAccessibleDevice::~MemoryMappedAccessibleDevice()
    {
    }

    /************************************BEGIN**************************************
     *
     * Function:    setBaseAddress
     *
     * Description:
     *//**
     *      Specify (or respecify) the device base address after construction.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b baseAddress - the base address of the memory-mapped Asic device
     *
     * @b Outputs:
     *
     *    Class Data:
     *      @arg @b myBaseAddress -   set to baseAddress argument
     *
     * @returns
     *      None
     *
     *//*
     *************************************END***************************************
     */
    void MemoryMappedAccessibleDevice::setBaseAddress(uint32 baseAddress)
    {
        myBaseAddress = reinterpret_cast<void *>(baseAddress);
    }

    /************************************BEGIN**************************************
     *
     * Function:    getBaseAddress
     *
     * Description:
     *//**
     *      Return the base address of this device.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg None
     *
     * @b Outputs:
     *
     *    Class Data:
     *      @arg @b baseAddress - the base address of the memory-mapped Asic device
     *
     * @returns
     *      device base address
     *
     *//*
     *************************************END***************************************
     */
    uint32 MemoryMappedAccessibleDevice::getBaseAddress() const
    {
        return reinterpret_cast<uint32>(myBaseAddress);
    }

    /************************************BEGIN**************************************
     *
     * Function:    read()
     *
     * Description:
     *//**
     *      Overloaded functions to read registers within the device.  
     *      Multiple functions are provided for different register sizes.
     *
     *      Note the call to completePriorAccessesBeforeContinuing() macro in the code
     *      below.  This is present to ensure that any prior reads/write that may
     *      have been fetched but not yet executed are actually completed before 
     *      the code which follows.  This is particulary important when accessing 
     *      I/O with a pipelined architecture (like the PowerPC) which orders 
     *      and executes reads (loads) and writes (stores) separately.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be read.  
     *
     *    Class Data:
     *      @arg @b myBaseAddress - the base address of this memory-mapped device
     *
     *    Global:
     *      @arg A memory-mapped register in the device
     *
     * @b Outputs:
     *
     *    @arg None
     *
     * @returns
     *    The data read from the specified register from the device.
     *
     *//*
     *************************************END***************************************
     */
    uint8 MemoryMappedAccessibleDevice::read(const volatile uint8 &deviceReg) const
    {
        uint8 data = *reinterpret_cast<const volatile uint8 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg));

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

        return data;
    }

    /** @overload */
    uint16 MemoryMappedAccessibleDevice::read(const volatile uint16 &deviceReg) const
    {
        uint16 data = *reinterpret_cast<const volatile uint16 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg));

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

        return data;
    }

    /** @overload */
    uint32 MemoryMappedAccessibleDevice::read(const volatile uint32 &deviceReg) const
    {
        uint32 data = *reinterpret_cast<const volatile uint32 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg));

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows

        return data;
    }

    /************************************BEGIN**************************************
     *
     * Function:    write()
     *
     * Description:
     *//**
     *      Overloaded functions to write registers within the device.
     *      Multiple functions are provided for different register sizes.
     *
     *      Note the call to completePriorAccessesBeforeContinuing() macro in the code
     *      below.  This is present to ensure that any prior reads/write that may
     *      have been fetched but not yet executed are actually completed before 
     *      the code which follows.  This is particulary important when accessing 
     *      I/O with a pipelined architecture (like the PowerPC) which orders 
     *      and executes reads (loads) and writes (stores) separately.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.  
     *      @arg @b data      - the data to write
     *
     *    Class Data:
     *      @arg @b myBaseAddress - the base address of this memory-mapped device
     *
     * @b Outputs:
     *
     *    Global:
     *      @arg A memory mapped register within the device.
     *
     * @returns
     *    None
     *
     *//*
     *************************************END***************************************
     */
    void MemoryMappedAccessibleDevice::write(volatile uint8 &deviceReg, uint8 data)
    {
        *reinterpret_cast<volatile uint8 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)) = data;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
    }

    /** @overload */
    void MemoryMappedAccessibleDevice::write(volatile uint16 &deviceReg, uint16 data)
    {
        *reinterpret_cast<volatile uint16 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)) = data;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
    }

    /** @overload */
    void MemoryMappedAccessibleDevice::write(volatile uint32 &deviceReg, uint32 data)
    {
        *reinterpret_cast<volatile uint32 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)) = data;

        completePriorAccessesBeforeContinuing(); // ensure the above read or write completes
                                                 //   before any code which follows
    }

} // namespace Devices
