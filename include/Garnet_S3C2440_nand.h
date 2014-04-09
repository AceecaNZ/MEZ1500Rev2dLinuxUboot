/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440_nand.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 NAND Flash controller registers
 *
 *****************************************************************************/

#ifndef __S3C2440A_NAND_H
#define __S3C2440A_NAND_H

typedef struct  
{
	UInt32	NFCONF;       	// 0x4E000000 NAND flash configuration
	UInt32	NFCONT;       	// 0x4E000004 NAND flash control      
	 UInt8	NFCMD;        	// 0x4E000008 NAND flash command      
	 UInt8	d0[3];        	//  
	 UInt8	NFADDR;       	// 0x4E00000C NAND flash address
	 UInt8	d1[3];        	//  
	UInt32	NFDATA;       	// 0x4E000010 NAND flash data
	UInt32	NFMECCD0;     	// 0x4E000014 NAND flash main area ECC0/1        
	UInt32	NFMECCD1;     	// 0x4E000018 NAND flash main area ECC2/3        
	UInt32	NFSECCD;      	// 0x4E00001C NAND flash spare area ECC          
	UInt32	NFSTAT;       	// 0x4E000020 NAND flash operation status        
	UInt32	NFESTAT0;     	// 0x4E000024 NAND flash ECC status for I/O[7:0] 
	UInt32	NFESTAT1;     	// 0x4E000028 NAND flash ECC status for I/O[15:8]
	UInt32	NFMECC0;      	// 0x4E00002C NAND flash main area ECC0 status   
	UInt32	NFMECC1;      	// 0x4E000030 NAND flash main area ECC1 status   
	UInt32	NFSECC;       	// 0x4E000034 NAND flash spare area ECC status   
	UInt32	NFSBLK;       	// 0x4E000038 NAND flash start block address     
	UInt32	NFEBLK;       	// 0x4E00003C NAND flash end block address       
	
} HwrS3C2440_NAND_RegType, *HwrS3C2440_NAND_RegPtr;    


/************************************************************************
 * NAND Flash configuration Register NFCONF bits
 ************************************************************************/
// TWRPH0 determines the read and write pulse widths
// duration = HCLK x (TWRPH0 + 1)
// HCLK = 100MHz (10ns period)
// For K9F1G08U WE* and WE* pulses must be 15ns minimum width
// so we need to set to 20ns (2 x HCLK)
// so TWRPH0 = 001

#define TACLS			2 
#define TWRPH0		1
#define TWRPH1		0

#define	bmaskNFCONFtacls		(3 << 12)		// [13:12] = TACLS
#define	bmaskNFCONFtwrph0		(7 << 8)		// [10:8] = TWRPH0
#define	bmaskNFCONFtwrph1		(7 << 4)		// [6:4] = TWRPH1

#define	bmaskNFCONFtiming		(bmaskNFCONFtacls | bmaskNFCONFtwrph0 | bmaskNFCONFtwrph1)
#define	bNFCONFtimingValues	((TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 4))	


/************************************************************************
 * NAND Flash Control Register NFCONT bits
 ************************************************************************/

#define	bNFCONTenable						BIT0		// MODE, 1 = enable NAND controller
#define	bNFCONTnFCE							BIT1		// nFCE control, 0 = nFCE low
#define	bNFCONTinitECC					BIT4		// 
#define	bNFCONTmainECClock			BIT5		// 
#define	bNFCONTspareECClock			BIT6		// 
#define	bNFCONTRnBtransmode			BIT7		// RnB transition detection, 0 = rising, 1 = falling edge
#define	bNFCONTRnBintEn					BIT8		// RnB transition interrupt enable
#define	bNFCONTillegalAccIntEn	BIT9		// illegal access interrupt enable
#define	bNFCONTsoftlock					BIT12		// soft lock enable
#define	bNFCONTlocktight				BIT13		// lock toght enable

/************************************************************************
 * NAND Flash Status Register NFSTAT bits
 ************************************************************************/

#define	bNFSTAT_RnB						BIT0		// RnB input status, 0= busy, 1 = ready
#define	bNFSTAT_nCE						BIT1		// nFCE output status
#define	bNFSTAT_RnBtrans			BIT2		// RnB transition (write 1 to clear)
#define	bNFSTAT_IllAccess			BIT3		// Illegal access detected


#define	NAND_NFDATA_REG				0x4E000010

#endif	// __S3C2440A_NAND_H
