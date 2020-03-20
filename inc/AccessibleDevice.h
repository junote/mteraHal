#ifndef ACCESSIBLE_DEVICE_HH
#define ACCESSIBLE_DEVICE_HH
/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2002 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    AccessibleDevice.hh
 *
 *      Declaration of the base AccessibleDevice class.
 *      This base class provides code for all of the common functions that
 *      an accessible device uses. These functions are the atomics, write
 *      fields, debug reads, and debug writes. The read() and write()
 *      methods are pure wirtual since they must be implemented by the
 *      derived class.
 *
 *      Note that this is an abstract base class (it contains a pure-virtual
 *      method) and cannot be directly instantiated.
 *
 * Type:    C++ include
 *//*
 *
 * NOTE:
 *      Comments in this file use Doxygen style formatting and Doxygen
 *      formatting tags. See the documentation at http://www.doxygen.org
 *      for more information about the style and use of formatting tags.
 *
 *************************************END***************************************
 */

#include <sstream>
#include "ss_gdef.h"                    // Tellabs standard data types

// Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Name:    AccessibleDevice
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
    class AccessibleDevice
    {
    friend class AggregateDevice;

    public:
        virtual ~AccessibleDevice() {}

        //----------------------------------------------------------------------
        // Debug functions - not for use by application code
        //
        //
        // Low-level direct reads/write - supported for DEBUG use only!!!!
        //   i.e. No application code should use these!!!!!
        //
        //   They are made public (vs. protected) so application code which finally
        //   instantiates a logical device can use them.
        //
        //   Again - only for debug purposes, not for use by app code.
        //
        virtual uint8 debugRead(const volatile uint8 &deviceReg) const;
        virtual uint16 debugRead(const volatile uint16 &deviceReg) const;
        virtual uint32 debugRead(const volatile uint32 &deviceReg) const;
        virtual void debugWrite(volatile uint8 &deviceReg, uint8 data);
        virtual void debugWrite(volatile uint16 &deviceReg, uint16 data);
        virtual void debugWrite(volatile uint32 &deviceReg, uint32 data);

        //
        // Same as above, only these take the register argument as a number
        // which represents the offset of the register from the device's base
        // address.  These read() functions can't be overloaded since they
        // differ only in the return type.  The write() functions could be
        // overloaded, but are not to be consistant with the read functions.
        //
        // read 8, 16, or 32 bits given a register offset
        virtual uint8 debugReadAtOffset8(uint32 offset) const;
        virtual uint16 debugReadAtOffset16(uint32 offset) const;
        virtual uint32 debugReadAtOffset32(uint32 offset) const;
        //
        // write 8, 16, or 32 bits given a register offset
        //
        virtual void debugWriteAtOffset8(uint32 offset, uint8 data);
        virtual void debugWriteAtOffset16(uint32 offset, uint16 data);
        virtual void debugWriteAtOffset32(uint32 offset, uint32 data);
        //
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Pure virtual functions - derived class must supply implementation
        //

        // return the instance number for a specific device
        virtual uint getInstanceNumber(void) const = 0;


        //
        //----------------------------------------------------------------------

    protected:
        //
        // The read/write functions below are the ones derived classes should
        // be using (vs. the debugRead/Write() above).  These are made protected
        // instead of public to prevent the final app code form doing low-level
        // accesses.  Only classes within the device model should do so.  App
        // code must use higher level methods provided in the derived classes to
        // access the hardware.
        //
        // Overloaded read functions to access various register sizes.
        // These are made virtual so they can be overridded by a derived class.
        // One reason you may want to do this is to handle reads and writes
        // for PCI devices, which require byte swapping to handle endian issues.
        virtual uint8 read(const volatile uint8 &deviceReg)     const = 0;
        virtual uint16 read(const volatile uint16 &deviceReg)   const = 0;
        virtual uint32 read(const volatile uint32 &deviceReg)   const = 0;

        // overloaded write functions to access various register sizes
        virtual void write(volatile uint8 &deviceReg, uint8 data) = 0;
        virtual void write(volatile uint16 &deviceReg, uint16 data) = 0;
        virtual void write(volatile uint32 &deviceReg, uint32 data) = 0;

        // atomic operations - won't be preempted/interrupted in mid-stride
        virtual void orAtomic(volatile uint8 &deviceReg, uint8 data);
        virtual void orAtomic(volatile uint16 &deviceReg, uint16 data);
        virtual void orAtomic(volatile uint32 &deviceReg, uint32 data);

        virtual void andAtomic(volatile uint8 &deviceReg, uint8 data);
        virtual void andAtomic(volatile uint16 &deviceReg, uint16 data);
        virtual void andAtomic(volatile uint32 &deviceReg, uint32 data);

        virtual void xorAtomic(volatile uint8 &deviceReg, uint8 data);
        virtual void xorAtomic(volatile uint16 &deviceReg, uint16 data);
        virtual void xorAtomic(volatile uint32 &deviceReg, uint32 data);

        virtual void writeFieldAtomic(volatile uint8 &deviceReg,
            uint8 fieldData, uint8 fieldDef);
        virtual void writeFieldAtomic(volatile uint16 &deviceReg,
            uint16 fieldData, uint16 fieldDef);
        virtual void writeFieldAtomic(volatile uint32 &deviceReg,
            uint32 fieldData, uint32 fieldDef);

        virtual void hexDump(const volatile uint8 *address,
            const volatile uint8 *end_address, std::stringstream &replyStream);
        virtual void hexDump(const volatile uint16 *address,
            const volatile uint16 *end_address, std::stringstream &replyStream);
        virtual void hexDump(const volatile uint32 *address,
            const volatile uint32 *end_address, std::stringstream &replyStream);

        AccessibleDevice& getAccessibleReference();
    };
} // namespace Devices

#endif // ACCESSIBLE_DEVICE_HH
