/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2001 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    AccessibleDevice.cc
 *
 *      Implementation of the AccessibleDevice class. This base class
 *      provides code for all of the common functions that an accessible
 *      device uses. These functions are the atomics, write fields, debug
 *      reads, and debug writes.
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
#include <stdio.h>
#include "ss_gdef.h"                      // Tellabs standard data types
#include "AccessibleDevice.h"             // declaration for AccessibleDevice
                                          // class
#include "PlatformDependent.h"            // For completePriorAccessesBeforeContinuing()
// #include "cCritSect.h"                    // Critical section class - used by
                                          // the "atomic" functions below

//** Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Function:    orAtomic()
     *
     * Description:
     *//**
     *      Overloaded functions to OR a register with a data pattern.
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.
     *      @arg @b data      - the data to be ORed into the register
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
    void AccessibleDevice::orAtomic(volatile uint8 &deviceReg, uint8 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint8 origRegContents = read(deviceReg);
        write(deviceReg, data | origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::orAtomic(volatile uint16 &deviceReg, uint16 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint16 origRegContents = read(deviceReg);
        write(deviceReg, data | origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::orAtomic(volatile uint32 &deviceReg, uint32 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint32 origRegContents = read(deviceReg);
        write(deviceReg, data | origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /************************************BEGIN**************************************
     *
     * Function:    andAtomic()
     *
     * Description:
     *//**
     *
     *      Overloaded functions to AND a register with a data pattern.
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.
     *      @arg @b data      - the data to be ANDed into the register
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
    void AccessibleDevice::andAtomic(volatile uint8 &deviceReg, uint8 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint8 origRegContents = read(deviceReg);
        write(deviceReg, data & origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::andAtomic(volatile uint16 &deviceReg, uint16 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint16 origRegContents = read(deviceReg);
        write(deviceReg, data & origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::andAtomic(volatile uint32 &deviceReg, uint32 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint32 origRegContents = read(deviceReg);
        write(deviceReg, data & origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /************************************BEGIN**************************************
     *
     * Function:    xorAtomic()
     *
     * Description:
     *//**
     *      Overloaded functions to XOR a register with a data pattern.
     *      This can be used to compliment bits in a register.
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.
     *      @arg @b data      - the data to be XORed into the register
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
    void AccessibleDevice::xorAtomic(volatile uint8 &deviceReg, uint8 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint8 origRegContents = read(deviceReg);
        write(deviceReg, data ^ origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::xorAtomic(volatile uint16 &deviceReg, uint16 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint16 origRegContents = read(deviceReg);
        write(deviceReg, data ^ origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::xorAtomic(volatile uint32 &deviceReg, uint32 data)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint32 origRegContents = read(deviceReg);
        write(deviceReg, data ^ origRegContents);

        // critSect.Exit();        // allow pre-emption
    }

    /************************************BEGIN**************************************
     *
     * Function:    writeFieldAtomic()
     *
     * Description:
     *//**
     *      Overloaded functions to write to a field within a register and leave
     *      the other bits unaltered.
     *
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - device register, i.e. a reference to the register to be
     *                  written to.
     *      @arg @b fieldData - field data, i.e. the data to be written into the register field
     *      @arg @b fieldDef -  field definition, i.e.  portion of register to be altered,
     *                  where :
     *                      a 1 in the fieldDef indicates the corresponding register
     *                              bit is to be written, and
     *                      a 0 indicates the bit should remain unaltered.
     *
     * @b Outputs:
     *
     *    Global:
     *      @arg A memory mapped register within the device.
     *
     * @returns
     *    None
     *
     * @b Example @b Usage:
     *      To set the upper 4 bits in an 8-bit register to 0011 and leave the other
     *      bits unaffected, do the following:
     *          writeFieldAtomic(someRegister, 0x30, 0xF0);
     *
     *//*
     *************************************END***************************************
     */
    void AccessibleDevice::writeFieldAtomic(volatile uint8 &deviceReg,
                                                             uint8 fieldData,
                                                             uint8 fieldDef)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint8 regCopy = read(deviceReg);        // read register
        regCopy &= ~fieldDef;                   // clear out bits to be affected
        write(deviceReg,
	    (fieldData & fieldDef) | regCopy);  // OR in new bits and write to register

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::writeFieldAtomic(volatile uint16 &deviceReg,
                                                             uint16 fieldData,
                                                             uint16 fieldDef)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint16 regCopy = read(deviceReg);       // read register
        regCopy &= ~fieldDef;                   // clear out bits to be affected
        write(deviceReg,
	    (fieldData & fieldDef) | regCopy);  // OR in new bits and write to register

        // critSect.Exit();        // allow pre-emption
    }

    /** @overload */
    void AccessibleDevice::writeFieldAtomic(volatile uint32 &deviceReg,
                                                             uint32 fieldData,
                                                             uint32 fieldDef)
    {
        // CCritSect critSect;

        // critSect.Enter();       // disable pre-emption

        uint32 regCopy = read(deviceReg);       // read register
        regCopy &= ~fieldDef;                   // clear out bits to be affected
        write(deviceReg,
	    (fieldData & fieldDef) | regCopy);  // OR in new bits and write to register

        // critSect.Exit();        // allow pre-emption
    }

    /************************************BEGIN**************************************
     *
     * Function:    debugRead()
     *
     * Description:
     *//**
     *      Overloaded functions to read registers within the device.    These
     *      are essentially the same as the read() method, except that debugRead()
     *      is a public function, and is provided ONLY for quick debugging purposes.
     *      No application code should use these!
     *
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be read.
     *
     *    Global:
     *      @arg A memory-mapped register in the device
     *
     * @b Outputs:
     *
     *    None
     *
     * @returns
     *    The data read from the specified register from the device.
     *
     *//*
     *************************************END***************************************
     */
    uint8 AccessibleDevice::debugRead(const volatile uint8 &deviceReg) const
    {
        return read(deviceReg);
    }

    /** @overload */
    uint16 AccessibleDevice::debugRead(const volatile uint16 &deviceReg) const
    {
        return read(deviceReg);
    }

    /** @overload */
    uint32 AccessibleDevice::debugRead(const volatile uint32 &deviceReg) const
    {
        return read(deviceReg);
    }

    /************************************BEGIN**************************************
     *
     * Function:    debugWrite()
     *
     * Description:
     *//**
     *      Overloaded functions to write registers within the device.    These
     *      are essentially the same as the write() method, except that debugWrite()
     *      is a public function, and is provided ONLY for quick debugging purposes.
     *      No application code should use these!
     *
     *      Multiple functions are provided for different register sizes.
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b deviceReg - a reference to the register to be written  to.
     *      @arg @b data      - the data to write
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
    void AccessibleDevice::debugWrite(volatile uint8 &deviceReg, uint8 data)
    {
        write(deviceReg, data);
    }

    /** @overload */
    void AccessibleDevice::debugWrite(volatile uint16 &deviceReg, uint16 data)
    {
        write(deviceReg, data);
    }

    /** @overload */
    void AccessibleDevice::debugWrite(volatile uint32 &deviceReg, uint32 data)
    {
        write(deviceReg, data);
    }

    /************************************BEGIN**************************************
     *
     * Function:    debugReadAtOffset8, 16, 32()
     *
     * Description:
     *//**
     *      Read 8, 16, or 32 bits from a device for debug purposes.
     *
     *      These functions differ from the debugRead() functions in that
     *      the register to be read is specifed as a simple number representing
     *      the register's offset from the device's base address.
     *
     *      With the debugRead() functions, the register to be read is specified
     *      as the symbolic register name within the structure, for the
     *      following simple device:
     *
     *      <pre>
     *      struct Regs
     *      {
     *          volatile const bits8    status;           // R
     *          volatile       bits8    control;          // R/W
     *          volatile       uint16   errorCounter;     // R/W
     *      };
     *
     *      Regs *const regs = 0;
     *      </pre>
     *
     *      the following calls read the contents of the "control" register:
     *
     *          <pre>
     *          uint8 data = debugRead(regs->control)
     *
     *          uint8 data = debugReadAtOffset8(1);     // since "control" is at offset 1
     *          </pre>
     *
     *      Note that debugReadAtOffset() is unsafe compared to debugRead().  This is
     *      because debugReadAtOffset() has no information about the register being read
     *      to determine its validity, data size, or if it is read-only.
     *      So you could, for example, attempt to read at an offset that is
     *      not even within the device, or attempt to read a 16 bit value from an 8 bit
     *      register.  In short, you should know what you're doing if you use this!
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b regOffset - the offset of the register from the device's base address.
     *
     *    Class Data:
     *      @arg @b myBaseAddress - the base address of this memory-mapped device
     *
     *    Global:
     *
     *      A memory-mapped register in the device
     *
     * @b Outputs:
     *
     *    None
     *
     * @returns
     *    The data read from the specified register from the device.
     *
     *//*
     *************************************END***************************************
     */
    uint8 AccessibleDevice::debugReadAtOffset8(uint32 offset) const
    {
        // This code looks strange because it uses the read() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint8 *reg = reinterpret_cast<uint8 *>(offset);
        return read(*reg);
    }

    /** @overload */
    uint16 AccessibleDevice::debugReadAtOffset16(uint32 offset) const
    {
        // This code looks strange because it uses the read() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint16 *reg = reinterpret_cast<uint16 *>(offset);
        return read(*reg);
    }

    /** @overload */
    uint32 AccessibleDevice::debugReadAtOffset32(uint32 offset) const
    {
        // This code looks strange because it uses the read() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint32 *reg = reinterpret_cast<uint32 *>(offset);
        return read(*reg);
    }

    /************************************BEGIN**************************************
     *
     * Function:    debugWriteAtOffset8, 16, 32()
     *
     * Description:
     *//**
     *      Write 8, 16, or 32 bits to a device for debug purposes.
     *
     *      These functions differ from the debugWrite() functions in that
     *      the register to be written is specifed as a simple number representing
     *      the register's offset from the device's base address.
     *
     *      With the debugWrite() functions, the register to be written is specified
     *      as the symbolic register name within the structure, for the
     *      following simple device:
     *
     *      <pre>
     *      struct Regs
     *      {
     *          volatile const bits8    status;           // R
     *          volatile       bits8    control;          // R/W
     *          volatile       uint16   errorCounter;     // R/W
     *      };
     *
     *      Regs *const regs = 0;
     *      </pre>
     *
     *      the following calls writes the contents of the "control" register:
     *
     *          <pre>
     *          debugWrite(regs->control, 0xFF)
     *
     *          debugWriteAtOffset8(1, 0xFF);       // since "control" is at offset 1
     *          </pre>
     *
     *      Note that debugWriteAtOffset() is unsafe compared to debugWrite().  This is
     *      because debugWriteAtOffset() has no information about the register being written
     *      to determine its validity, data size, or if it is read-only.
     *      So you could, for example, attempt to write at an offset that is
     *      not even within the device, or attempt to write a 16 bit value to an 8 bit
     *      register, or even attempt to write to a read-only (i.e. const) register.
     *      In short, you should know what you're doing if you use this!
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b regOffset - the offset of the register from the device's base address.
     *      @arg @b data      - the data to write
     *
     *    Class Data:
     *      @arg @b myBaseAddress - the base address of this memory-mapped device
     *
     * @b Outputs:
     *
     *    Global:
     *
     *      A memory mapped register within the device.
     *
     * @returns
     *    None
     *
     *//*
     *************************************END***************************************
     */
    void AccessibleDevice::debugWriteAtOffset8(uint32 offset, uint8 data)
    {
        // This code looks strange because it uses the write() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint8 *reg = reinterpret_cast<uint8 *>(offset);
        write(*reg, data);
    }

    /** @overload */
    void AccessibleDevice::debugWriteAtOffset16(uint32 offset, uint16 data)
    {
        // This code looks strange because it uses the write() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint16 *reg = reinterpret_cast<uint16 *>(offset);
        write(*reg, data);
    }

    /** @overload */
    void AccessibleDevice::debugWriteAtOffset32(uint32 offset, uint32 data)
    {
        // This code looks strange because it uses the write() method, which expects
        // the register to be read to be in the form of a reference to a uint{8, 16, or 32}.
        // We need to form this argument by creating a local variable and setting its
        // address to the specified (structure) offset.
        uint32 *reg = reinterpret_cast<uint32 *>(offset);
        write(*reg, data);
    }

    /************************************BEGIN**************************************
     *
     * Function:    hexDump()
     *
     * Description:
     *//**
     *      Prints the specified range of addresses in hex dump format
     *
     * @b Inputs:
     *
     *    Formal:
     *      @arg @b address     - the start address of the hex dump
     *      @arg @b end_address - the end address of the hex dump
     *      @arg @b replyStream - stream where the hex dump is printed to
     *
     *    Class Data:
     *      None
     *
     * @b Outputs:
     *
     *    Global:
     *      None
     *
     * @returns
     *    None
     *
     *//*
     *************************************END***************************************
     */
    void AccessibleDevice::hexDump(const volatile uint8 *address,
                                   const volatile uint8 *end_address,
                                   std::stringstream &replyStream)
    {
        // make sure that start is on a doubleword boundary
        const volatile uint8 *actual_start = address;

        if(actual_start < end_address)
        {
            char outBuf[128];
            int  columnCount = 0;

            while(address <= end_address)
            {
                if((columnCount % 16) == 0)
                {
                    if(!address)
                    {
                        // address 0 cannot be printed with %p
                        snprintf(outBuf, sizeof(outBuf), "    0x00: ");
                    }
                    else
                    {
                        snprintf(outBuf, sizeof(outBuf), "%8p: ", address);
                    }
                    replyStream << std::endl << outBuf;
                }

                if(address < actual_start)
                {
                    replyStream << "   ";
                }
                else
                {
                    snprintf(outBuf, sizeof(outBuf), "%02x ", static_cast<uint>(read(*address)));
                    replyStream << outBuf;
                }
                ++address;
                ++columnCount;
            }
            replyStream << std::endl;
        }
    }

    /** @overload */
    void AccessibleDevice::hexDump(const volatile uint16 *address,
                                   const volatile uint16 *end_address,
                                   std::stringstream &replyStream)
    {
        // make sure that start is on a doubleword boundary
        const volatile uint16 *actual_start = address;

        if(actual_start < end_address)
        {
            char outBuf[128];
            int  columnCount = 0;

            while(address <= end_address)
            {
                if((columnCount % 16) == 0)
                {
                    if(!address)
                    {
                        // address 0 cannot be printed with %p
                        snprintf(outBuf, sizeof(outBuf), "    0x00: ");
                    }
                    else
                    {
                        snprintf(outBuf, sizeof(outBuf), "%8p: ", address);
                    }
                    replyStream << std::endl << outBuf;
                }

                if(address < actual_start)
                {
                    replyStream << "   ";
                }
                else
                {
                    snprintf(outBuf, sizeof(outBuf), "%04x ", static_cast<uint>(read(*address)));
                    replyStream << outBuf;
                }
                ++address;
                ++columnCount;
            }
            replyStream << std::endl;
        }
    }

    /** @overload */
    void AccessibleDevice::hexDump(const volatile uint32 *address,
                                   const volatile uint32 *end_address,
                                   std::stringstream &replyStream)
    {
        // make sure that start is on a doubleword boundary
        const volatile uint32 *actual_start = address;

        if(actual_start < end_address)
        {
            char outBuf[128];
            int  columnCount = 0;

            while(address <= end_address)
            {
                if(columnCount % 16 == 0)
                {
                    if(!address)
                    {
                        // address 0 cannot be printed with %p
                        snprintf(outBuf, sizeof(outBuf), "    0x00: ");
                    }
                    else
                    {
                        snprintf(outBuf, sizeof(outBuf), "%8p: ", address);
                    }
                    replyStream << std::endl << outBuf;
                }

                if(address < actual_start)
                {
                    replyStream << "   ";
                }
                else
                {
                    snprintf(outBuf, sizeof(outBuf), "%08x ", static_cast<uint>(read(*address)));
                    replyStream << outBuf;
                }
                ++address;
                ++columnCount;
            }
            replyStream << std::endl;
        }
    }

    /************************************BEGIN**************************************
     *
     * Function:    getAccessibleReference()
     *
     * Description:
     *//**
     *      Converts the _this_ pointer to a reference for use in derived ctors
     *
     * @b Inputs:
     *
     *    Formal:
     *      None
     *    Class Data:
     *      None
     *
     * @b Outputs:
     *
     *    Global:
     *      None
     *
     * @returns
     *    Reference to self
     *
     *//*
     *************************************END***************************************
     */
    AccessibleDevice& AccessibleDevice::getAccessibleReference()
    {
      return *this;
    }
} // namespace Devices
