/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2001 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    AggregateDevice.cpp
 *
 *      Implementation of the AggregateDevice class
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

#include "ss_gdef.h"           // Tellabs standard data types
#include "AggregateDevice.h"   // declaration for AggregateDevice class

// Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Function:    AggregateDevice
     *
     * Description:
     *//**
     *      Constructor for AggregateDevice class - establishes
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
    AggregateDevice::AggregateDevice(AccessibleDevice& baseDevice, void *baseAddress) :
        myAccessibleDevice(baseDevice),
        myBaseAddress(baseAddress)
    {
        // We need to check to ensure that myBaseDevice != this
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
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be read.  
     *
     *    Class Data:
     *      @arg @b myAccessibleDevice -  the accessible device for our aggregator
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
    uint8 AggregateDevice::read(const volatile uint8 &deviceReg) const
    {
        return(myAccessibleDevice.read(*reinterpret_cast<const volatile uint8 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg))));
    }

    /** @overload */
    uint16 AggregateDevice::read(const volatile uint16 &deviceReg) const
    {
        return(myAccessibleDevice.read(*reinterpret_cast<const volatile uint16 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg))));
    }

    /** @overload */
    uint32 AggregateDevice::read(const volatile uint32 &deviceReg) const
    {
        return(myAccessibleDevice.read(*reinterpret_cast<const volatile uint32 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg))));
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
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.  
     *      @arg @b data      - the data to write
     *
     *    Class Data:
     *      @arg @b myAccessibleDevice - the accessible device for our aggregator
     *
     * @b Outputs:
     *
     *    @arg None
     *
     * @returns
     *    None
     *
     *//*
     *************************************END***************************************
     */
    void AggregateDevice::write(volatile uint8 &deviceReg, uint8 data)
    {
        myAccessibleDevice.write(*reinterpret_cast<volatile uint8 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)),
            data);
    }

    /** @overload */
    void AggregateDevice::write(volatile uint16 &deviceReg, uint16 data)
    {
        myAccessibleDevice.write(*reinterpret_cast<volatile uint16 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)),
            data);
    }

    /** @overload */
    void AggregateDevice::write(volatile uint32 &deviceReg, uint32 data)
    {
        myAccessibleDevice.write(*reinterpret_cast<volatile uint32 *>
            (reinterpret_cast<uint32>(myBaseAddress) + reinterpret_cast<uint32>(&deviceReg)),
            data);
    }

} // namespace Devices
