/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: nand.h
 *
 * Release: 
 *
 * Description:
 *					Nand read & write functions used for backup and recovery
 *
 *****************************************************************************/
#ifndef __NAND_H
#define __NAND_H

//#include "S3C2440Hwr.h"
#include <common.h>


typedef unsigned int 		Err;
typedef unsigned int 		Boolean;
typedef unsigned int 		UInt32;
typedef unsigned char 	UInt8;
typedef unsigned short 	UInt16;

#define	false		0
#define	true		1


#include "Garnet_sysdefs.h"
#include "Garnet_S3C2440Hwr.h"
#include "Garnet_S3C2440_nand.h"



// Standard NAND flash commands 
#define NAND_CMD_READ0					0x00
#define NAND_CMD_RANDOMOUT1			0x05
#define NAND_CMD_PAGEPROG				0x10
#define NAND_CMD_CACHEDPROG			0x15
#define NAND_CMD_READSTART			0x30
#define NAND_CMD_ERASE1					0x60
#define NAND_CMD_STATUS					0x70
//#define NAND_CMD_STATUS_MULTI		0x71
#define NAND_CMD_SEQIN					0x80
#define NAND_CMD_RANDOMIN				0x85
#define NAND_CMD_READID					0x90
#define NAND_CMD_ERASE2					0xD0
#define NAND_CMD_RANDOMOUT2			0xE0
#define NAND_CMD_RESET					0xFF

// Samsung NAND Flash Manufacturer ID Code 
#define NAND_MFR_SAMSUNG				0xEC

#define	NAND_BLOCK_SIZE					0x20000		// 128KB/block for K9F1G08 128MB flash
#define	NAND_PAGE_SIZE					0x800			// 2KB/page for K9F1G08 128MB flash
#define	NAND_OOB_SIZE_PER_PAGE	64				// Each page has 64 bytes of OOB
#define	NAND_PAGE_WORDS					(NAND_PAGE_SIZE/4)					// 32-bit words/page
#define	NAND_OOB_WORDS					(NAND_OOB_SIZE_PER_PAGE/4)	// 32-bit words/OOB

#define	NAND_BB_OFFSET					0x800			// OOB start for K9F1G08 128MB flash
#define	NAND_PAGES_PER_BLOCK		64

// Last word of OOB reserved for ECC
#define	NAND_ECC_WORD_IN_OOB		0x0F

// Error codes
#define	NAND_ERR_NONE				0
#define	NAND_ERR_BADPARAM		1		// NULL pointer
#define	NAND_ERR_TIMEOUT		2		// timeout waiting for RnB (ready not busy) to go high
#define	NAND_ERR_ILLEGAL		3		// illegal access (attempt to write/erase write protected NAND)
#define	NAND_ERR_FAIL				4		// erase/write failed with NAND STATUS bit 0 = 1 (FAIL)
#define	NAND_ERR_ECC				5		// read failed due to incorrect ECC

// Bad block markers
// For testing, use a different marker depnding on reason for block failure
#define NAND_BAD_FACTORY  0x0
#define	NAND_BAD_READ			0x0BAD0ECC
#define	NAND_BAD_WRITE		0x1BAD1BAD
#define	NAND_BAD_ERASE		0x2BADEEEE


// Protypes
Err 	Nand_BlockRead (UInt32 blockAddr, UInt32 *pDest);
Err 	Nand_BlockWrite (UInt32 blockAddr, UInt32 *pSrc);

void	Nand_Init (void);
Err 	Nand_Reset (void);
Boolean 	Nand_PageIsErased (UInt32 address);
Boolean 	Nand_BlockIsErased (UInt32 blockAddr);
Err 	Nand_BlockErase (UInt32 blockAddr);
Err 	Nand_PageRead (UInt32 address, UInt32 *pBuf,  UInt32 *pOobBuf);
Err		Nand_PageWrite (UInt32 address, UInt32 *pBuf,  UInt32 *pOobBuf);

//Err 	Nand_Read (UInt32 dest, UInt32 src, UInt32 size);
//Err 	Nand_Write (UInt32 src, UInt32 dest, UInt32 size);

//Boolean page_isbad(UInt32 address);
Boolean block_isbad(UInt32 blockAddr);
void Nand_MarkBad (UInt32 blockAddr, UInt32 marker);

void DbgOut(char *s);
void DbgNum(UInt32 x);

#endif // __NAND_H
