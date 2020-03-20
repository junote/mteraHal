#ifndef DEVICES_PLATFORM_DEPENDENT_HH
#define DEVICES_PLATFORM_DEPENDENT_HH
/************************************BEGIN**************************************
 *                        CONFIDENTIAL AND PROPRIETARY
 *             Copyright (C) 2002 Tellabs, All Rights Reserved
 *******************************************************************************
 *
 * File:    PlatformDependent.hh
 * Type:    C++ include
 * Description:
 *      Platform dependent information used by the device library.
 *
 *************************************END***************************************
 */

/************************************BEGIN**************************************
 *
 * Function:    completePriorAccessesBeforeContinuing()
 *
 * Description:
 *      Ensure that preceding instructions have completed execution before
 *      continuing, which may be necessary for pipelined architecture 
 *      like the PowerPC.
 *
 *      The PowerPC divides loads and stores into two sets, each of which is
 *      ordered separately.  Problems can occur if a store must be completed
 *      before a load, but the instructions get executed in a different order.
 *
 *      For the PowerPC, the "eieio" instruction can be used to ensure that
 *      any applicable memory accesses preceding the "eieio" instruction complete
 *      before those which follow.  This is particulary useful to ensure that 
 *      a store completes before a load instruction by separating them by 
 *      an "eieio".
 *
 *      Note that we must prevent the compiler from reordering assembly code when 
 *      optimization is enabled.  See the "Embedding Assembly Code" section in
 *      the Diab compiler manual for the reason why.  
 *
 *      If your platform doesn't alter execution ordering or it isn't an issue, 
 *      then this function doesn't need to do anything.  This might be the case
 *      when doing device simulation on a Solaris workstation, for example.
 *
 * Inputs:
 *      Compiler macros:
 *          PPC - must be defined if target platform is a PowerPC
 *
 * Outputs:
 *      inserts code to ensure in-order execution
 *
 * Returns:
 *      None
 *
 *************************************END***************************************
 */
#ifdef PPC
/* for PowerPC, define an assembly macro to do the work */

#ifdef MVL
/* 
** The original code for this macro contained comments as shown:
** 
** #define completePriorAccessesBeforeContinuing() \
** asm("    .set noreorder    # stop optimizer from messing with us"); \
** asm("    eieio             # enforce in-order execution for I/O"); \
** asm("    .set reorder      # let optimizer do its thing again");
**
** However, the MVL gnu assembler flagged errors on the .set 
** lines, so they have been removed until I can determine the root cause
** - arossett 11/13/2006
**
** The GNU assembler does not do instruction scheduling and does not 
** support the .set no/reorder directives. So this is the correct solution
** for MVL builds.
** - arossett 04/27/2007
**
*/

#define completePriorAccessesBeforeContinuing() \
asm("    eieio             # enforce in-order execution for I/O"); 

#else  // MVL
#ifdef WRLINUX

/* for now, putting this in for WRLINUX */
#define completePriorAccessesBeforeContinuing() \
asm("    eieio             # enforce in-order execution for I/O"); 

#else

#define completePriorAccessesBeforeContinuing() \
asm("    .set noreorder    # stop optimizer from messing with us"); \
asm("    eieio             # enforce in-order execution for I/O"); \
asm("    .set reorder      # let optimizer do its thing again");

#endif  // WRLINUX
#endif  // MVL

#else   // PPC

/* null definition for all other platforms */
#define completePriorAccessesBeforeContinuing()

#endif // PPC


#endif // DEVICES_PLATFORM_DEPENDENT_HH
