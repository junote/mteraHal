// BEGIN COPYRIGHT *****************************************
//
// Copyright 2015 Coriant GmbH. All rights reserved
//
// The reproduction, transmission or use of this document
// or its contents is not permitted without express written
// authority. Offenders may be held liable for damages. All
// rights, including rights created by patent grant or
// registration of a utility model or design, are reserved.
//
// Technical modifications possible.
//
// Technical specifications and features are binding only
// insofar as they are specifically and expressly agreed
// upon in a written contract.
//
// END COPYRIGHT *******************************************

#ifndef MBCPLDSPI_H
#define MBCPLDSPI_H

#include <linux/ioctl.h>

#define SPI_DEVICE_NAME     "mbCpldSpi"

/*---------------------------------------------------------------------*/
/*            Serial Peripheral Controller (SPI) Symbols               */
/*---------------------------------------------------------------------*/
#define SPI_CS_N          (1 << 31)
#define SPI_LOOP          (1 << 30)
#define SPI_CLK_INVERT    (1 << 29)  // Not present in OSM4F CPLD
#define SPI_CLK_PHASE     (1 << 28)  // Not present in OSM4F CPLD
#define SPI_CLK_DIV16     (1 << 27)  // Not present in OSM4F CPLD
#define SPI_NORM_DATA     (1 << 26)  // Not present in OSM4F CPLD
#define SPI_MASTER        (1 << 25)  // Not present in OSM4F CPLD
#define SPI_ENABLE        (1 << 24)

#define SPI_LEN_SHIFT     20
#define SPI_LEN_FIELD     (0xf << SPI_LEN_SHIFT)
#define SPI_PM_BIT3       (1 << 19) /* DIV16 */   // Not present in OSM4F CPLD
#define SPI_PM_BIT2       (1 << 18) /* DIV8  */   // Not present in OSM4F CPLD
#define SPI_PM_BIT1       (1 << 17) /* DIV4  */   // Not present in OSM4F CPLD
#define SPI_PM_BIT0       (1 << 16) /* DIV2  */   // Not present in OSM4F CPLD

#define SPI_SPIE_LT       (1 << 14)
#define SPI_SPIE_OV       (1 << 12)
#define SPI_SPIE_NE       (1 << 9)
#define SPI_SPIE_NF       (1 << 8)

#define SPI_SLAVE_SELECT_SHIFT  16
#define SPI_SLAVE_SELEC_FIELD   (0xf << SPI_SLAVE_SELECT_SHIFT)

/*---------------------------------------------------------------------*/
/* SPI Definitions                                                     */
/*---------------------------------------------------------------------*/

#define SPI_TIMEOUT  1000
#define SPI_FREQ    1000000  /* Desired SPI freq used to calculate SPI_PM */
#define MAXRXBUFFERLGTH 256 + 4  /* the IDPROM needs '64 + 3' for datas */
                                 /* instruction and address.            */
                                 /* the ADC needs only 3 for datas and  */
                                 /* instruction.                        */

/*---------------------------------------------------------------------*/
/* Error codes that may be returned by this driver                     */
/*---------------------------------------------------------------------*/
#define SPI_BUSY    0x10060003  /* Spi driver already in use */
#define SPI_OINV    0x10060004  /* invalid device # in open call */
#define SPI_CINV    0x10060005  /* invalid device # in close call */
#define SPI_RECL    0x10060006  /* receive buffer length invalid  */
#define SPI_INIT    0x10060007  /* SPI not initialized */
#define SPI_XFRS    0x10060008  /* transfer problem encountered */
#define SPI_LGTH    0x10060009  /* SPI Rx or Tx buffer length too large */

#define SPI_OK    0  /* no error in the SPI call */
#define SPI_FAIL -1  /* error in the SPI call */

#define SPI_PM_VALUE (10 << 16)  /* PM = [BRGCLK/(SPI_FREQ)*4]-1 */
                                 /* {BRGCLK = 333333333/8}, SPI_FREQ = 1000000  */

/*---------------------------------------------------------------------*/
/* SPI stats structure                                                 */
/*---------------------------------------------------------------------*/
typedef struct
{
    unsigned int lt_lastCharXmtd;
    unsigned int ov_bufferOverrun;
    unsigned int ne_rxNotEmpty;
    unsigned int nf_txNotFull;
} SPI_STATS;

typedef struct
{
    void  *txBuffPtr;  /* pointer to transmit buffer */
    void  *rxBuffPtr;  /* pointer to receive buffer */
    unsigned short txBuffLen;  /* transmit buffer length */
    unsigned short rxBuffLen;  /* receive buffer length */
} SPI_DATA;

#define SEMAPHORE_FLAG      (1 << 16)
#define CACHE_LINE_SIZE          0x10

/*
 * Split minors in two parts
 */
#define TYPE(minor) (((minor) >> 4) & 0xf)  /* high nibble */
#define NUM(minor)  ((minor) & 0xf)  /* low  nibble */

#define SPI_IOCTL_MAGIC 'g'


typedef struct
{
    unsigned char txBuffPtr[MAXRXBUFFERLGTH + (2 * CACHE_LINE_SIZE)];  /* pointer to transmit buffer */
    unsigned char rxBuffPtr[MAXRXBUFFERLGTH + (2 * CACHE_LINE_SIZE)];  /* pointer to receive buffer */
    unsigned short txBuffLen;  /* transmit buffer length */
    unsigned short rxBuffLen;  /* receive buffer length */
} spiDataArgType;

#define SPI_IOCTL_SPI_IO _IOWR(SPI_IOCTL_MAGIC, 40, SPI_DATA *)

typedef struct
{
    unsigned long theMode;
    unsigned long spiFreq;
} spiSetModeArgType;

#define SPI_IOCTL_SET_SPI_MODE _IOWR(SPI_IOCTL_MAGIC, 10, spiSetModeArgType *)

typedef struct
{
    unsigned int lt_lastCharXmtd;
    unsigned int ov_bufferOverrun;
    unsigned int ne_rxNotEmpty;
    unsigned int nf_txNotFull;
    unsigned short clearStats;
} spiGetErrStatsArgType;

#define SPI_IOCTL_GET_ERR_STATS  _IOWR(SPI_IOCTL_MAGIC, 11, spiGetErrStatsArgType *)

/* Argument to use for cpldSet/GetChiSel and associated ioctl */
#define SPI_CS_DESELECT   0
#define SPI_CS_SELECT     1

/* Argument to use for cpldSet/GetChipSel and associated ioctl */
#define SPI_DEVICE_DIGI1_SPI_FLASH          0x0
#define SPI_DEVICE_DIGI2_SPI_FLASH          0x01
#define SPI_DEVICE_DIGI3_SPI_FLASH          0x02
#define SPI_DEVICE_GE_SWITCH                0x03
#define SPI_DEVICE_SFPPC_AS_CPLD            0x04
#define SPI_DEVICE_SFPPC_PROM_CPLD          0x05

#define SPI_DEVICE_DIGI1_SPI_SLAVE_CS0      0x06
#define SPI_DEVICE_DIGI1_SPI_SLAVE_CS1      0x07
#define SPI_DEVICE_DIGI2_SPI_SLAVE_CS0      0x08
#define SPI_DEVICE_DIGI2_SPI_SLAVE_CS1      0x09
#define SPI_DEVICE_GX_SPI_SWITCH            0X0A

#define SPI_DEVICE_NONE                     0x0F

#define SPI_DEVICE_DIGI1_CS0_SELECT         0x0
#define SPI_DEVICE_DIGI1_CS1_SELECT         0x01
#define SPI_DEVICE_DIGI2_CS0_SELECT         0x02
#define SPI_DEVICE_DIGI2_CS1_SELECT         0x03
#define SPI_DEVICE_GX_SWITCH_SELECT         0X04

#define SPI_DEVICE_GE_SWITCH_SELECT         0x05
#define SPI_DEVICE_DIGI1_FLASH_SELECT       0x06
#define SPI_DEVICE_DIGI2_FLASH_SELECT       0x07
#define SPI_DEVICE_DIGI3_FLASH_SELECT       0x08
#define SPI_DEVICE_SFPPC_AS_CPLD_SELECT     0x0d
#define SPI_DEVICE_SFPPC_PROM_CPLD_SELECT     0x0c
#define SPI_DEVICE_SELECT_NONE              0x0f

#define SPI_DEVICE_ALL        255
#define SPI_DEVICE_MAX        7

typedef union
{
    spiDataArgType spiDataA;
    spiSetModeArgType spiModeA;
    spiGetErrStatsArgType spiStatsA;
    unsigned char spiCsA;
} spiIoctlArgType;

typedef struct
{
    unsigned int spiDevice;
} spiSlaveSelArgType;

typedef struct
{
    unsigned int spiDevice;
    unsigned int spiCs;
} spiChipSelArgType;



#define SPI_IOCTL_SET_CHIPSEL             _IOWR(SPI_IOCTL_MAGIC, 30, spiSlaveSelArgType *)
#define SPI_IOCTL_GET_CHIPSEL             _IOWR(SPI_IOCTL_MAGIC, 31, spiSlaveSelArgType *)

#define SPI_IOCTL_SET_SPISLAVE            _IOWR(SPI_IOCTL_MAGIC, 32, spiChipSelArgType *)
#define SPI_IOCTL_GET_SPISLAVE            _IOWR(SPI_IOCTL_MAGIC, 33, spiChipSelArgType *)

#endif
