/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440_lcd.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 LCD registers
 *
 *****************************************************************************/

#ifndef __S3C2440A_LCD_H
#define __S3C2440A_LCD_H

typedef struct {
    UInt32 LCDCON1;             // 0x4D000000								
    UInt32 LCDCON2;             // 0x4D000004								
    UInt32 LCDCON3;             // 0x4D000008								
    UInt32 LCDCON4;             // 0x4D00000C								
    UInt32 LCDCON5;             // 0x4D000010								
    UInt32 LCDSADDR1;           // 0x4D000014								
    UInt32 LCDSADDR2;           // 0x4D000018								
    UInt32 LCDSADDR3;           // 0x4D00001C								
    UInt32 REDLUT;              // 0x4D000020								
    UInt32 GREENLUT;            // 0x4D000024								
    UInt32 BLUELUT;             // 0x4D000028								
    UInt32 PAD[8];              // 0x4D00002C - 0x0x4D000048								
    UInt32 DITHMODE;            // 0x4D00004C								
    UInt32 TPAL;                // 0x4D000050								
    UInt32 LCDINTPND;           // 0x4D000054								
    UInt32 LCDSRCPND;           // 0x4D000058								
    UInt32 LCDINTMSK;           // 0x4D00005C   								
    UInt32 TCONSEL;             // 0x4D000060		
    						
} HwrS3C2440_LCD_RegType, *HwrS3C2440_LCD_RegPtr;


/*
	From Hitachi TX09D40VM3CAA LCD datasheet
	Vsync frequency typ 60 Hz (54 - 68 Hz)
	Hsync frequency typ 20.77 kHz (18.57 - 22.73 kHz)
	DCLK frequency  typ 5.6 MHz (5.0 - 6.5 MHz)
	HBP typ 12 * dotclock
	HFP typ 18 (15 - 21) * dotclock
	VBP typ 8 * HSYNC
	VFP typ 20 (17 - 22) * HSYNC
	
	Backlight turn on delay min 2 frames after video
	
	Frame time = (VSPW+1 + VBPD+1 + LINEVAL+1 + VFPD+1)*(HSPW+1 + HBPD+1 + HOZVAL+1 + HFPD+1)* 2*(CLKVAL+1)/HCLK
	
	Dotclock frequency = (HCLK/2)/(CLKVAL+1) = 5.6 Mhz
	HCLK/2 = 50MHz so require CLKVAL = 8 for 5.6 MHz dot clock
	1/(5.6MHz) = 178 ns / pixel
	
	VSPW+1 =  
	VBPD+1 = 8
	LINEVAL+1 = 320
	VFPD+1 = 20
	HSPW+1 = 
	HBPD+1 = 12
	HOZVAL+1 = 240
	HFPD+1 = 18

	Put VSPW+1 = 1 and HSPW+1 = 5
	Frame time = (1 + 8 + 320 + 20) * (5 + 12 + 240 + 18) * 178 ns = 16.7 ms (1/60Hz)
						 = (349) * (275) * 178 ns = 17.1 ms (58.5 Hz)
*/

#define	LCD_XSIZE		240		// horizontal LCD size in pixels
#define	LCD_YSIZE		320		// vertical LCD size in pixels
#define	LCD_BPP			16		// bits / pixel


/************************************************************************
 * LCD Control 1 Register LCDCON1 bits
 ************************************************************************/

#define	LCDCON1stn1bpp		0		// STN, 1 bpp
#define	LCDCON1stn2bpp		1		// STN, 2 bpp
#define	LCDCON1stn4bpp		2		// STN, 4 bpp
#define	LCDCON1stn8bpp		3		// STN, 8 bpp
#define	LCDCON1stnp12bpp	4		// STN, packed 12 bpp
#define	LCDCON1stnu12bpp	5		// STN, unpacked 12 bpp
#define	LCDCON1stn16bpp		6		// STN, 16 bpp
#define	LCDCON1tft1bpp		8		// TFT, 1 bpp
#define	LCDCON1tft2bpp		9		// TFT, 2 bpp
#define	LCDCON1tft4bpp		10	// TFT, 4 bpp
#define	LCDCON1tft8bpp		11	// TFT, 8 bpp
#define	LCDCON1tft16bpp		12	// TFT, 16 bpp
#define	LCDCON1tft24bpp		13	// TFT, 24 bpp

#define	LCDCON1_VCLK			8		// VCLK = HCLK / ((CLKVAL + 1) * 2)
															// HCLK = 100MHz => VCLK = 50MHz/9 = 5.6 Mhz

// Bit shift values
#define bshftLCDCON1bppmode	1		// [4:1] BPPMODE
#define bshftLCDCON1pnrmode	5		// [6:5] PNRMODE
#define bshftLCDCON1clkval	8		// [17:8] CLKVAL
#define bshftLCDCON1linecnt	18	// [27:18] LINECNT, readonly

// Bit field masks
#define bmaskLCDCON1bppmode	(0x0F << 1)		// [4:1] BPPMODE
#define bmaskLCDCON1pnrmode	(3 << 5)			// [6:5] PNRMODE
#define bmaskLCDCON1clkval	(0x3FF << 8)	// [17:8] CLKVAL
#define bmaskLCDCON1linecnt	(0x3FF << 18)	// [27:18] LINECNT, readonly

// Bit masks
#define bLCDCON1envid		BIT0	// ENVID enable video
#define bLCDCON1mmode		BIT7	// MMODE, toggle rate, 0 = each frame, 1 = determine by MVAL

// Initialisation values as bit masks
#define	bLCDCON1tft16bpp	(LCDCON1tft16bpp << 1)	// BPPMODE = TFT, 16 bpp
#define bLCDCON1pnrTFT		(3 << 5)								// PNRMODE = TFT LCD Pannel
#define	bLCDCON1clkval		(LCDCON1_VCLK << 8)			// CLKVAL = VCLK

// Set up LCDCON1 for TFT panel with ENVID = 0 (LCD output disabled)
// MMODE = 0, VM signal toggles each frame, MVAL not used
// Overall initialisation value
#define	bLCDCON1_init		(bLCDCON1clkval | bLCDCON1pnrTFT |	bLCDCON1tft16bpp)

/************************************************************************
 * LCD Control 2 Register LCDCON2 bits
 ************************************************************************/

// Initialisation values
#define	LCDCON2_VSPW			1							// VSPW TFT vert sync pulse width
#define	LCDCON2_VFPD			20						// VFPD, TFT vertical front porch = inactive lines at end of frame before vert sync
#define	LCDCON2_LINEVAL		(LCD_YSIZE-1)	// LINEVAL = vertical size of panel (in pixels)
#define	LCDCON2_VBPD			8							// VBPD = TFT vert back porch = inactive lines at frame start after vert sync

// Bit shift values
#define bshftLCDCON2vspw			0		// [5:0] VSPW
#define bshftLCDCON2vfpd			6		// [13:6] VFPD
#define bshftLCDCON2lineval		14	// [23:14] LINEVAL
#define bshftLCDCON2vbpd			24	// [31:24] VBPD

// Bit field masks
#define bmaskLCDCON2vspw			(0x3F << 0)		// [5:0] VSPW
#define bmaskLCDCON2vfpd			(0xFF << 6)		// [13:6] VFPD
#define bmaskLCDCON2lineval		(0x3FF << 14)	// [23:14] LINEVAL
#define bmaskLCDCON2vbpd			(0xFF << 24)	// [31:24] VBPD

// Initialisation values as bit masks
#define	bLCDCON2_VSPW			(LCDCON2_VSPW << 0)			// VSPW
#define	bLCDCON2_VFPD			(LCDCON2_VFPD << 6)			// VFPD
#define	bLCDCON2_LINEVAL	(LCDCON2_LINEVAL << 14)	// LINEVAL
#define	bLCDCON2_VBPD			(LCDCON2_VBPD << 24)		// VBPD

// Overall initialisation value
#define	bLCDCON2_init		(bLCDCON2_VBPD | bLCDCON2_LINEVAL | bLCDCON2_VFPD | bLCDCON2_VSPW)

/************************************************************************
 * LCD Control 3 Register LCDCON3 bits
 ************************************************************************/

// Initialisation values
#define	LCDCON3_HFPD		15						// [7:0] HFPD = horiz front porch = VCLK periods from end of active data till HSYNC /
#define	LCDCON3_HOZVAL	(LCD_XSIZE-1)	// [18:8] HOZVAL = horizontal size of LCD in pixels
#define	LCDCON3_HBPD		5							// [25:19] HBPD = horiz back porch = VCLK periods from HSYNC \ till start of active data

// Bit shift values
#define	bshftLCDCON3hfpd		0		// [7:0] HFPD
#define	bshftLCDCON3hozval	8		// [18:8] HOZVAL
#define	bshftLCDCON3hbpd		19	// [25:19] HBPD

// Bit field masks
#define	bmaskLCDCON3hfpd		(0x3FF << 0)	// [7:0] HFPD
#define	bmaskLCDCON3hozval	(0x7FF << 8)	// [18:8] HOZVAL
#define	bmaskLCDCON3hbpd		(0x7F << 19)	// [25:19] HBPD

// Initialisation values as bit masks
#define	bLCDCON3_HFPD		(LCDCON3_HFPD << 0)		// [7:0] HFPD
#define	bLCDCON3_HOZVAL	(LCDCON3_HOZVAL << 8)	// [18:8] HOZVAL
#define	bLCDCON3_HBPD		(LCDCON3_HBPD << 19)	// [25:19] HBPD

// Overall initialisation value
#define	bLCDCON3_init		(bLCDCON3_HBPD | bLCDCON3_HOZVAL | bLCDCON3_HFPD)

/************************************************************************
 * LCD Control 4 Register LCDCON4 bits
 ************************************************************************/

// Initialisation values
#define	LCDCON4_HSPW	5	// [7:0] HSPW = horiz sync pulse width = # of VCLK in HSYNC high period
#define	LCDCON4_MVAL	0	// [15:8] MVAL = rate of VM (for MMODE = 1), 0 = reset value, NOT USED (MMODE = 0)

// Bit shift values
#define	bshftLCDCON4hspd		0	// [7:0] HSPW
#define	bshftLCDCON4mval		8	// [15:8] MVAL

// Bit field masks
#define	bmaskLCDCON4hspd		(0xFF << 0)	// [7:0] HSPW
#define	bmaskLCDCON4mval		(0xFF << 8)	// [15:8] MVAL

// Initialisation values as bit masks
#define	bLCDCON4_HSPW	(LCDCON4_HSPW << 0)	// [7:0] HSPW
#define	bLCDCON4_MVAL	(LCDCON4_MVAL << 8)	// [15:8] MVAL

// Overall initialisation value
#define	bLCDCON4_init		(bLCDCON4_HSPW |bLCDCON4_MVAL)

/************************************************************************
 * LCD Control 5 Register LCDCON5 bits
 ************************************************************************/

// Bit shift values
#define bshftLCDCON5hstatus	13	// [14:13] HSTATUS = horizontal status, read only
#define bshftLCDCON5vstatus	15	// [16:15] VSTATUS = vertical status, read only

// Bit field masks
#define bmaskLCDCON5hstatus	(3 << 13)	// [14:13] HSTATUS
#define bmaskLCDCON5vstatus	(3 << 15)	// [16:15] VSTATUS

// Bit masks
#define bLCDCON5hwswp			BIT0	// HWSWP = half word swap enable
#define bLCDCON5bswp			BIT1	// BSWP	= byte swap enable
#define bLCDCON5enlend		BIT2	// ENLEND	= LEND output enable
#define bLCDCON5pwren			BIT3	// PWREN = PWREN output enable
#define bLCDCON5invlend		BIT4	// INVLEND = invert LEND
#define bLCDCON5invpwren	BIT5	// INVPWREN = invert PWREN
#define bLCDCON5invvden		BIT6	// INVVDEN = invert VDEN
#define bLCDCON5invvd		 	BIT7	// INVVD = invert VD
#define bLCDCON5invvframe	BIT8	// INVVFRAME = invert VFRAME/VSYNC
#define bLCDCON5invvline	BIT9	// INVVLINE = invert VLINE/HSYNC
#define bLCDCON5invvclk 	BIT10	// INVVCLK = VCLK active edge 1 = rising
#define bLCDCON5frm565		BIT11	// FRM565 = TFT format 0 = 5:5:5:1, 1 = 5:6:5
#define bLCDCON5bpp24bl		BIT12	// BPP24BL = order of 24bpp memory, 1 = MSb valid

// Initialisation value
#define	bLCDCON5_init		(bLCDCON5frm565 | bLCDCON5invvclk |bLCDCON5invvline | bLCDCON5invvframe | bLCDCON5pwren | bLCDCON5hwswp)

//------------------------------------------------------------------------------

	// Notes on LCD frame buffer addressing
	// LCD data is always a multiple of 4 bytes (32-bits)
	// However pagesize is defined in half-words (16-bit units)
	// and data is transferred to LCD in 16-bit chunks
	// so the LSB of address is not required
	// For single scan LCD, LCDSADDR1 = A[30:1] of start address of LCD frame buffer
	// The 9 MSbits (A[29:21] of LCDSADDR1) are the bank (LCDBANK)
	// the 21 LSbits (A20:0] of LCDSADDR1) are the start address of the frame within the bank (LCDBASEU)
	//
	// The LCD DMA controller requires these addresses to be physical addreses not virtual

/************************************************************************
 * LCD Frame buffer start address Register LCDSADDR1
 ************************************************************************/
 
// Bit shift values
#define bshftLCDSADDR1baseu	0		// [20:0] LCDBASEU A[21:1] of start address of frame buffer
#define bshftLCDSADDR1bank	21	// [29:21] LCDBANK A[30:22] of video buffer

// Bit field masks
#define bmaskLCDSADDR1baseu		(0x1FFFFF << 0)	// [20:0] LCDBASEU
#define bmaskLCDSADDR1bank		(0x1FF << 21)		// [29:21] LCDBANK

// Initialisation values
// 2010-02-25 HD: Added definition here to change the BASEU to a secondary screen
// when used for double buffering.
#define LCDSADDR1_BASEU_SECOND	(0x80000 >> 1) 
#define LCDSADDR1_BASEU_FIRST		0
#define	LCDSADDR1_BANK					(hwrPhysicalDisplayBase >> 1)

// Initialisation value (note the BASEU will change later once the OS is running and screen locking/double buffering functions are used)
#define	bLCDSADDR1_init		(LCDSADDR1_BANK | LCDSADDR1_BASEU_FIRST)

/************************************************************************
 * LCD Frame buffer start address Register LCDSADDR2
 ************************************************************************/

/*
	// LCD frame size (in halfwords) 
			= frame end address - frame start address
	 		= LCD actual height (pixel rows) x virtual screen width (half-words)
	
	Note that each row of the virtual screen is (PAGEWIDTH + OFFSIZE) halfwords long
	LCDFRAMESIZE 
						= (PAGEWIDTH+OFFSIZE) x (LINEVAL+1)
						= (PAGEWIDTH+OFFSIZE) x LCD_YSIZE
						= (240+0) * 320
						= 76800
						= 0x12C00
	 
	LCDBASEL	= (LCDSADDR1_BASEU_SECOND or LCDSADDR1_BASEU_FIRST) + LCDFRAMESIZE
*/
#define LCDFRAMESIZE	(LCDSADDR3_PAGEW + LCDSADDR3_OFFSIZE)*LCD_YSIZE	// in halfwords

//#define	LCDSADDR2_BASEL		(LCDSADDR1_BASEU + LCDFRAMESIZE)

// Bit shift values
#define bshftLCDSADDR2basel	0								// [20:0] LCDBASEL A[21:1] of end address of frame buffer

// Bit field masks
#define bmaskLCDSADDR2basel	(0x1FFFFF << 0)	// [20:0] LCDBASEL


/************************************************************************
 * LCD Frame buffer start address Register LCDSADDR3
 ************************************************************************/

// Bit shift values
#define bshftLCDSADDR3pagew		0			// [10:0] PAGEWIDTH = virtual screen page width
#define bshftLCDSADDR3offsize	11		// [21:11] OFFSIZE = virtual screen offset size (half words)
																	// = # of offset for one line

// NOTE: The values of PAGEWIDTH and OFFSIZE must be changed only when ENVID bit is 0

// OFFSIZE determines how much wider the virtual screen is than the actual screen so is the amount
// by which screen can be scrolled horizontally.
// ******** HORIZONTAL SCROLLING IS NOT SUPPORTED BY GARNET OS ********
// Note.
// Since OFFSIZE = 0 this means the virtual screen buffer has the same width as the physical screen
// which in turn means that the number of bytes of data for 1 row of the screen is simply:
//		RowBytes = LCD_XSIZE * LCD_BPP / 8
 
// Initialisation values
#define LCDSADDR3_PAGEW			(LCD_XSIZE*16/16)	// [10:0] PAGEWIDTH = virtual screen page width (half words)
#define LCDSADDR3_OFFSIZE		0									// [21:11] OFFSIZE = virtual screen offset size (half words)

// Initialisation values as bit masks
#define	bLCDSADDR3_PAGEW		(LCDSADDR3_PAGEW << 0)		// PAGEWIDTH
#define	bLCDSADDR3_OFFSIZE	(LCDSADDR3_OFFSIZE << 11)	// OFFSIZE

// Initialisation value
#define	bLCDSADDR3_init		(bLCDSADDR3_OFFSIZE | bLCDSADDR3_PAGEW)


//------------------------------------------------------------------------------

// THIS IS HERE TO KEEP THE COMPILER HAPPY
// BUT THE ASOCIATED CODE HAS NOT BEEN VISITED YET
// see PrvGetHwrPaletteEntry and PrvSetHwrPaletteEntry in HALDisplay.c
// and PrvMX1InitLUT in HwrDisplayBootS3C2440.c

// color map RAM defines
#define hwrMX1LcdColorMapRamStart             0x03100800
#define hwrMX1LcdColorMapRamEnd               0x03100C00



#endif	// __S3C2440A_LCD_H

