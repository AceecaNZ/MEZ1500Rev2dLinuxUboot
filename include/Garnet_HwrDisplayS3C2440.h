/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: HwrDisplayS3C2440.h
 *
 * Release: 
 *
 * Description:
 *		Header file for video controller hardware abstraction layer.
 *
 *****************************************************************************/

#ifndef __HWRDISPLAYS3C2440_H__
#define __HWRDISPLAYS3C2440_H__


/*****************************************************************
 * Constants
 *****************************************************************/

// EF: testing SPI timing

#define	DBI_CS_DLY	3		// us


#define hwrDisplayType				'S3C'
#define hwrDisplayRevision		1
#define hwrDisplayVersion			1


#define LCD_TYPE_H35									1	// Hitachi TX09D40VM3CAA, 240x3(RGB)(W)x320(H), TFT
#define LCD_TYPE_MTECH_32_320x480			1	// Microtech MTF0320CPIL-07, 3.2in, 320x480, TFT

#define hwrLCDType		LCD_TYPE_MTECH_32_320x480

#ifndef hwrInternalSRAM
	#define hwrDisplayVRAMExternal		1
#endif

// *****************************************************************
// FOR TFT COLOR PANEL
// *****************************************************************
#if (hwrLCDType == LCD_TYPE_H35)
	#define	hwrLCDWidth                	240
	#define	hwrLCDHeight               	320
                                       	
	// Depth Info                      	
	#define hwrDisplayBootDepth        	16  		// Boot time bits per pixel (system default)	
	#define hwrDisplayMaxDepth         	16  		// Maximum bits per pixel hardware supports
	#define hwrDisplayMaxPaletteEntries	256 		// Maximum number of CLUT entries.
	                                   	
	#define hwrDisplayAllDepths        	0x8088	// Bit depths 16,8,4 are supported, so bits 16,8,4 are set in the constant
	#define hwrDisplayMaxGrays	      	16   		// Maximum number of displayable grays.
	#define hwrDisplayColor			   			1     	// Device does supports color 
	                                   	
	#define hwrDisplayHSyncCfg         	0x04000000
	#define hwrDisplayVSyncCfg         	0x04000007

#elif (hwrLCDType == LCD_TYPE_MTECH_32_320x480)
	#define	hwrLCDWidth                	320
	#define	hwrLCDHeight               	480
                                       	
	// Depth Info                      	
	#define hwrDisplayBootDepth        	16  		// Boot time bits per pixel (system default)	
	#define hwrDisplayMaxDepth         	16  		// Maximum bits per pixel hardware supports
	#define hwrDisplayMaxPaletteEntries	256 		// Maximum number of CLUT entries.
	                                   	
	#define hwrDisplayAllDepths        	0x8088	// Bit depths 16,8,4 are supported, so bits 16,8,4 are set in the constant
	#define hwrDisplayMaxGrays	      	16   		// Maximum number of displayable grays.
	#define hwrDisplayColor			   			1     	// Device does supports color 
	                                   	
	#define hwrDisplayHSyncCfg         	0x04000000
	#define hwrDisplayVSyncCfg         	0x04000007
#endif


// *****************************************************************
// For 320 x 480 panel
// *****************************************************************

// Screen Dimensions
#define hwrDisplayVRAMExternal			1
#define hwrDisplayDoubleBuffer			1
#define hwrDisplayWidth							320		// Width of display (pixels)
#define hwrDisplayHeight	    			480		// Height of display (pixels)
#define hwrInputAreaHeight      		160
#define hwrDigitizer_to_std_scale		kFixedOneHalf
//#define hwrDigitizer_to_std_scale		kFixedTwoThirds

#ifdef hwrDisplayVRAMExternal
#define hwrDisplayVRAMReserved      0					// Top-of-eSRAM reserved for overlay and info
#define hwrDisplayVRAMSize          0x100000	// 1MBytes of external RAM for Display
#else
#define hwrDisplayVRAMReserved      0x3000		// Top-of-eSRAM reserved for overlay and info
#define hwrDisplayVRAMSize          (hwrInternalRAMSize - hwrDisplayVRAMReserved) // 116KBytes of in-HW eSRAM Memory for Display
#endif


#define hwrDigitizerWidth 	hwrDisplayWidth 	// Width of display
#define hwrDigitizerHeight	hwrDisplayHeight	// Height of display

#define hwrDisplayPeriod			17		// screen refresh rate, frame period in ms
																		// refer calculation in S3C2440_lcd.h

// use the VZ default value contrast. 
// In VZ it is defined that the Mid range for MAX1629 is 16, 16+10 = 26 (10 steps we set in HwrLCDInit)
// 26 map to 00-FF range the result is D7.
#define hwrDisplayContrastDefault	0xD7		// Contrast default value

/*****************************************************************
 * Function Prototypes
 *****************************************************************/
/*
// implemented in HwrDisplayBootS3C2440.c
void HALPrvDisplayCurGeometry(UInt16* widthP, UInt16* heightP, UInt16* depthP);
// The following function initialises display with default start address and depth
Err HALPrvDisplayInit(void);
// The following function is used to change the starting address and depth
Err HALPrvDisplayNew(void *newPhysBaseAddr, UInt16 newDepth);
*/
#endif	//  __HWRDISPLAYS3C2440_H__
