/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440Hwr.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 internal registers
 *
 *****************************************************************************/

#ifndef	__S3C2440HWR_H__
#define	__S3C2440HWR_H__

//#ifdef	NON_PORTABLE							// So apps don't include this...
/*
#include <PalmTypes.h>							// Palm includes

#include "sysdefs.h"								// BIT definitions

#include "S3C2440_adc.h"
#include "S3C2440_cam.h"
#include "S3C2440_clkp.h"
#include "S3C2440_dma.h"
#include "S3C2440_i2c.h"
#include "S3C2440_i2s.h"
#include "S3C2440_intr.h"
#include "S3C2440_gpio.h"
#include "S3C2440_lcd.h"
#include "S3C2440_memc.h"
#include "S3C2440_nand.h"
#include	"S3C2440_rtc.h"
#include "S3C2440_sdi.h"
#include "S3C2440_spi.h"
#include "S3C2440_tmr.h"
#include "S3C2440_uart.h"
#include "S3C2440_usbd.h"
#include "S3C2440_wdog.h"
*/
// Define virtual location of the Samsung S3C2440 internal registers
// for now the virtual address is same as physical address
#define	hwrS3C2440regBase			0x48000000L		// Virtual Base address of internal registers
#define	hwrS3C2440regPhysBase	0x48000000L		// Physical Base address of internal registers
#define	hwrS3C2440regSize			0x14000000L		// size of memory space for all CPU registers

#define hwrS3C2440_MEMC_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x48000000)
#define hwrS3C2440_USBH_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x49000000)
#define hwrS3C2440_INTR_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4A000000)
#define hwrS3C2440_DMA_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4B000000)
#define hwrS3C2440_CLKP_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4C000000)
#define hwrS3C2440_LCD_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4D000000)
#define hwrS3C2440_NAND_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4E000000)
#define hwrS3C2440_CAM_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x4F000000)
#define hwrS3C2440_UART0_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x50000000)
#define hwrS3C2440_UART1_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x50004000)
#define hwrS3C2440_UART2_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x50008000)
#define hwrS3C2440_TMR_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x51000000)
#define hwrS3C2440_USBD_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x52000000)
#define hwrS3C2440_WDOG_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x53000000)
#define hwrS3C2440_I2C_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x54000000)
#define hwrS3C2440_I2S_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x55000000)
#define hwrS3C2440_GPIO_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x56000000)
#define hwrS3C2440_RTC_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x57000000)
#define hwrS3C2440_ADC_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x58000000)
#define hwrS3C2440_SPI_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x59000000)
#define hwrS3C2440_SDI_Base		(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x5A000000)
#define hwrS3C2440_AC97_Base	(hwrS3C2440regBase - hwrS3C2440regPhysBase + 0x5B000000)
     
// common base for all 3 UARTs
#define hwrS3C2440_UARTS_Base	hwrS3C2440_UART0_Base



/************************************************************************
 * Access and naming conventions:
 *
 * For UARTs (since they are defined by an array of uart structures) the method of access is as follows:
 * e.g. to set Tx enable bit (bUCONtxEn) in UCON register of UART2
 *		HwrS3C2440_UARTS_Ptr 	baseP = (HwrS3C2440_UARTS_Ptr)hwrS3C2440_UARTS_Base;	// create a base pointer
 *		index = 2;																	// index selects the required uart
 *		baseP->uart[index].UCON |= bUCONtxEn;				// set the bit using bit mask
 *
 * All other the registers will be accessed via a pointer to the Base address of the corresponding register block
 *		HwrS3C2440_TMR_RegPtr tmrP = (HwrS3C2440_TMR_RegPtr)hwrS3C2440_TMR_Base;	// point to timer reg block
 *		tmrP->TCON = 0;													// write to TCON register in the tmr block
 *
 * S3C2440 register bit naming conventions:
 * Individual bits are defined as bit-masks using BIT0 - BIT31
 * and named bREGbitname
 * where
 * REG is the register name as in the S3C2440 datasheet
 * bitname is an arbitrary descriptive name for the bit
 * e.g.
 * 	#define bTCONt1start	BIT8				// timer 1 start/stop bit in TCON register, tmr block
 *
 *		tmrP->TCON |= bTCONt1start;			// start timer 1
 *		tmrP->TCON &= ~bTCONt1start;		// stop timer 1
 *
 * Multiple-bit fields are also defined as masks for setting separate values in the field and also
 * an overall field_mask which sets all bits in the field.
 * Normally the field will first be cleared and then the required value written as follows:
 *	baseP->blk.reg &= ~field_mask		// first clear all bits in field
 *	baseP->blk.reg |= bitmask				// then set just the required bits in field
 *
 * Naming of multiple-bit fields:
 * 	bfREGbitfieldname		// name for the entire field, all bits set in this field_mask
 *		bREGbitsvalue			// name for individual value for the field, 0 or more bits set
 *
 * e.g.
 * 	#define bfTCFG1mux3		(0xF << 12)		// MUX 3 field (4 bits) in TCFG1 register, tmr block
 * 	#define bTCFG1mux3div8	(0x2 << 12)	// MUX 3 divide by 8 = 0010
 *
 *		tmrP->TCFG1 &= ~bfTCFG1mux3;			// clear the MUX 3 field bits
 *		tmrP->TCFG1 |= bTCFG1mux3div8;		// then set the required value
 *
 * For fields that may hold a large number of possible vales, such as prescalers individual values are not defined
 * but an overall field mask is still defined (all bits set) to allow the entire field to be cleared (complement used)
 * and a bit shift value is defined for the field position within the register.
 * Naming of bit shift values:
 * 	bfREGbitfieldname		// name for the entire field, all bits set in this field_mask
 *		bsREGbitfieldname	// name of the field
 *
 * e.g.
 * 	#define bsTCFG0prescale1	8							// prescaler 1 field starts at 8 (LSB) in TCFG0 register, tmr block
 * 	#define bfTCFG0prescale1	(0xFF << bsTCFG0prescale1)
 *																					// prescaler 1 field is 8 bits wide = bits15:8
 *		#define PRESCALER1VALUE	25						// requires shifting !! into the field position
 *
 *		tmrP->TCFG0 &= ~bfTCFG0prescale1;											// clear the prescaler 1 field
 *		tmrP->TCFG0 |= (PRESCALER1VALUE << bsTCFG0prescale1);	// then set the required value
 *
 ************************************************************************/


// These don't yet have separate header files

// USB Host registers
typedef struct {
	UInt32	rsvd[23];
} HwrS3C2440_USBH_RegType, *HwrS3C2440_USBH_RegPtr;

// AC97 Codec registers
typedef struct {
	UInt32	AC_GLBCTRL;			// 0x5B000000 AC97 global control register     
	UInt32	AC_GLBSTAT;			// 0x5B000004 AC97 global status register      
	UInt32	AC_CODEC_CMD;		// 0x5B000008 AC97 codec command register      
	UInt32	AC_CODEC_STAT;	// 0x5B00000C AC97 codec status register       
	UInt32	AC_PCMADDR;		  // 0x5B000010 AC97 PCM out/in channel FIFO     
	UInt32	AC_MICADDR;			// 0x5B000014 AC97 mic in channel FIFO address 
	UInt32	AC_PCMDATA;			// 0x5B000018 AC97 PCM out/in channel FIFO data
	UInt32	AC_MICDATA;			// 0x5B00001C AC97 MIC in channel FIFO data    

} HwrS3C2440_AC97_RegType, *HwrS3C2440_AC97_RegPtr;



typedef struct {
	UInt32 cpuFreq;			// FCLK
	UInt32 hclkFreq;		// HCLK
	UInt32 pclkFreq;		// PCLK
	UInt32 uclkFreq;		// UCLK
} HwrS3C2440_ClocksType, *HwrS3C2440_ClocksPtr;

void HALGetSystemFreq( HwrS3C2440_ClocksPtr clockP ); 

/************************************************************************
 * ARM MMU Page Table Structures
 ************************************************************************/
typedef union {
	struct {
		UInt32	tag:2;
		UInt32	ignored:30;
	} faultEntry;
	struct {
		UInt32	tag:2;
		UInt32	implementationDefined:3;
		UInt32	domain:4;
		UInt32	shouldBeZero:1;
		UInt32	base:22;
	} coarseEntry;
	struct {
		UInt32	tag:2;
		UInt32	buffered:1;
		UInt32	cacheable:1;
		UInt32	implementationDefined:1;
		UInt32	domain:4;
		UInt32	shouldBeZero2:1;
		UInt32	accessPermissions:2;
		UInt32	shouldBeZero1:8;
		UInt32	base:12;
	} sectionEntry;
	struct {
		UInt32	tag:2;
		UInt32	implementationDefined:3;
		UInt32	domain:4;
		UInt32	shouldBeZero:3;
		UInt32	base:20;
	} fineEntry;
} FirstLevelTableEntry, *FirstLevelTableEntryPtr;

typedef union {
	struct {
		UInt32	tag:2;
		UInt32	ignored:30;
	} faultEntry;
	struct {
		UInt32	tag:2;
		UInt32	buffered:1;
		UInt32	cacheable:1;
		UInt32	accessPermissions0:2;
		UInt32	accessPermissions1:2;
		UInt32	accessPermissions2:2;
		UInt32	accessPermissions3:2;
		UInt32	shouldBeZero:4;
		UInt32	base:16;
	} largePageEntry;
	struct {
		UInt32	tag:2;
		UInt32	buffered:1;
		UInt32	cacheable:1;
		UInt32	accessPermissions0:2;
		UInt32	accessPermissions1:2;
		UInt32	accessPermissions2:2;
		UInt32	accessPermissions3:2;
		UInt32	base:20;
	} smallPageEntry;
	struct {
		UInt32	tag:2;
		UInt32	buffered:1;
		UInt32	cacheable:1;
		UInt32	accessPermissions:2;
		UInt32	shouldBeZero:4;
		UInt32	base:22;
	} tinyPageEntry;
} SecondLevelTableEntry, *SecondLevelTableEntryPtr;

typedef enum {
	FaultEntryType = 0x0,
	CoarseEntryType = 0x1,
	SectionEntryType = 0x2,
	FineEntryType = 0x3,
	LargePageType = 0x1,
	SmallPageType = 0x2,
	TinyPageType = 0x3
} PageTableEntryType;

#define CoarseEntryRequiredBits	0x4

#define BitsPerMegabyte	20
#define BitsPerPage	12


//#endif // NON_PORTABLE

#endif // __S3C2440HWR_H__
