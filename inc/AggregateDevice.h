#ifndef AGGREGATE_DEVICE_HH
#define AGGREGATE_DEVICE_HH
/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2000-2002 Tellabs, All Rights Reserved
 *******************************************************************************
 *//**
 *
 * @file    AggregateDevice.hh
 *
 *      Declaration of the base AggregateDevice class.
 *      This base class provides code for the read/write functions that
 *      an aggregate device uses.
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

#include "ss_gdef.h"                    // Tellabs standard data types
#include "AccessibleDevice.h"           // Base class for an accesible device
                                        // that contains methods common to all
                                        // accessible devices.

// Everything in the devices library will be contained in this namespace
namespace Devices
{
    /************************************BEGIN**************************************
     *
     * Name:    AggregateDevice
     *
     * Description:
     *//**
     *      Declaration for low-level driver for an aggregate device.  At this level,
     *      the driver only provides primitive read/write access to the various
     *      registers within the device.
     *
     *//*
     *************************************END***************************************
     */
    class AggregateDevice : public virtual AccessibleDevice
    {
        public:
            AggregateDevice(AccessibleDevice& baseDevice, void *baseAddress);
            // destructor  - should be virtual if any methods are
            virtual ~AggregateDevice() {}

            // deprecated
            virtual uint getInstanceNumber() const { return(0); }

        protected:
            virtual uint8 read(const volatile uint8 &deviceReg)     const;
            virtual uint16 read(const volatile uint16 &deviceReg)   const;
            virtual uint32 read(const volatile uint32 &deviceReg)   const;

            virtual void write(volatile uint8 &deviceReg, uint8 data);
            virtual void write(volatile uint16 &deviceReg, uint16 data);
            virtual void write(volatile uint32 &deviceReg, uint32 data);

            // Protected data
            AccessibleDevice& myAccessibleDevice;
            void *const       myBaseAddress;   // base offset of the sub-device

    };
} // namespace Devices

#endif // AGGREGATE_DEVICE_HH
