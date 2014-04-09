/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440_adc.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 ADC registers
 *
 *****************************************************************************/

#ifndef __S3C2440A_ADC_H
#define __S3C2440A_ADC_H

typedef struct {
	UInt32	ADCCON;   	// 0x58000000 control register
	UInt32	ADCTSC;   	// 0x58000004 touch screen control register
	UInt32	ADCDLY;   	// 0x58000008 start or interval delay register
	UInt32	ADCDAT0;  	// 0x5800000C conversion data register 0
	UInt32	ADCDAT1;  	// 0x58000010 conversion data register 1
	UInt32	ADCUPDN;		// 0x58000014 touch screen up-down int check
	
} HwrS3C2440_ADC_RegType, *HwrS3C2440_ADC_RegPtr;        




/************************************************************************
 * ADC Control Register ADCCON bits
 ************************************************************************/
// ADC channels
#define ADCCONch0		0		// ADC channel 0
#define ADCCONch1		1		// ADC channel 1
#define ADCCONch2		2		// ADC channel 2
#define ADCCONch3		3		// ADC channel 3
#define ADCCONchYM	4		// ADC channel YM
#define ADCCONchYP	5		// ADC channel YP
#define ADCCONchXM	6		// ADC channel XM
#define ADCCONchXP	7		// ADC channel XP

// Bit field masks - use (inverted) for clearing fields
#define bmaskADCCONch			(7 << 3)					// ADC channel (MUX) bits[5:3]
#define bmaskADCCONprscvl	(0xFF << 6)	// ADC prescaler bits[13:6]

// Bit masks - use for setting bits/bit fields
#define bADCCONenStart	BIT0	// enable ADC start
#define bADCCONrdStart	BIT1	// enable ADC read start
#define bADCCONstdby		BIT2	// standby mode
#define bADCCONprscen		BIT14	// prescaler enable
#define bADCCONecflg		BIT15	// end of conversion

#define bADCCONch0		(ADCCONch0	<< 3)	// ADC channel 0
#define bADCCONch1		(ADCCONch1	<< 3)	// ADC channel 1
#define bADCCONch2		(ADCCONch2	<< 3)	// ADC channel 2
#define bADCCONch3		(ADCCONch3	<< 3)	// ADC channel 3
#define bADCCONchYM		(ADCCONchYM << 3)	// ADC channel YM
#define bADCCONchYP		(ADCCONchYP << 3)	// ADC channel YP
#define bADCCONchXM		(ADCCONchXM << 3)	// ADC channel XM
#define bADCCONchXP		(ADCCONchXP << 3)	// ADC channel XP

// Note. (ref S3C2440 User manual Rev1.3 p16-5)
// When changing from standby mode to normal operation mode the prescaler
// must be enabled at least 3 ADC clocks in advance

/************************************************************************
 * ADC Touch Screen Control Register ADCTSC bits
 ************************************************************************/
  // Mode bits [1:0]
#define ADCTSCnoOp		0	// no operation mode
#define ADCTSCxPos		1	// X-position measurement
#define ADCTSCyPos		2	// Y-position measurement
#define ADCTSCwt4int	3	// waiting for interrupt operation mode (must be cleared after interrupt)

// Bit field mask
#define ADCTSCmode		3	// mode bits [1:0]


// Bit masks - use for setting bits/bit fields
#define bADCTSCautopst	BIT2	// AUTO_PST, auto sequence X, Y measurement
#define bADCTSCnPullup	BIT3	// PULL_UP, disable XP PULL_UP
#define bADCTSCnXP			BIT4	// XP_SEN, disable XP output driver
#define bADCTSCenXM			BIT5	// XM_SEN, enable XM output driver
#define bADCTSCnYP			BIT6	// YP_SEN, disable YP output driver
#define bADCTSCenYM			BIT7	// YM_SEN, enable YM output driver
#define bADCTSCstylusUp	BIT8	// UD_SEN, detect stylus up

#define bADCTSCsleep		(BIT6|BIT4|BIT3)											// all drivers off, no pullup, no op mode
#define bADCTSCwt4tap		(BIT7|BIT6|BIT4|ADCTSCwt4int)					// YM on,YP,XM,XP off, pullup, wait for stylus tap int
//#define bADCTSCautoXY		(BIT7|BIT6|BIT4|BIT3|BIT2|ADCTSCnoOp)	// auto measurement X,Y mode
// It appears that by setting BIT2 the status of BITs 6 & 7 are not important as they must be controlled automatically
#define bADCTSCautoXY		(BIT6|BIT4|BIT3|BIT2|ADCTSCnoOp)	// auto measurement X,Y mode
#define bADCTSCwt4up		(BIT8|BIT7|BIT6|BIT4|ADCTSCwt4int)		// YM on,YP,XM,XP off, pullup, wait for stylus up int

/************************************************************************
 * ADC Data Register ADCDATx bit
 ************************************************************************/
#define bADCDATstylusUp	BIT15	// UPDOWN, state of stylus at wait for int mode, 1 = up

/************************************************************************
 * ADC Touch Screen Up-Down Interrupt Check Register ADCUPDN bits
 ************************************************************************/
#define bADCUPDNdn	BIT0	// Stylus down interrupt occurred
#define bADCUPDNup	BIT1	// Stylus up interrupt occurred


/************************************************************************
 * ADC constants
 ************************************************************************/

// Note
// ADCDLY = ADC start delay in cycles of PCLK (not the ADC clock as expected and as advised by Samsung!)
// Note. (ref S3C2440 User manual Rev1.3 p16-5)
// When changing from standby mode to normal operation mode the prescaler
// must be enabled at least 3 ADC clocks in advance

// For Touch panel monitoring
#define ADCCON_PRSCVL_tp		49			// for touch panel ADC frequency = PCLK/(49+1) = 1 MHz
#define	ADCDLY_tp						250			// * 20 ns = 5 us
#define	ADCCON_initTP		(bADCCONprscen | (ADCCON_PRSCVL_tp << 6) | bADCCONchXP)
																		// PRSCEN=1,PRSCVL=49(1MHz),MUX=7=XP
																		
// before starting a conversion start in standby with prescaler disabled, but channel and prescaler setup
#define	ADCCON_stdbyTP		(bADCCONstdby | (ADCCON_PRSCVL_tp << 6) | bADCCONchXP)
// Then start the prescvaler by setting bADCCONprscen
// then wait 3 us
// Then switch to normal operation mode by clearing bADCCONstdby

// For battery monitor
#define ADCCON_PRSCVL_bv		49			// for battery V ADC frequency = PCLK/(49+1) = 1 MHz
#define	ADCDLY_bv						250			// * 20 ns = 5 us
#define	ADCCON_initBV		(bADCCONprscen | (ADCCON_PRSCVL_bv << 6) | bADCCONch0)
																		// PRSCEN=1,PRSCVL=49(1MHz),MUX=0=VBATT

#define	ADCCON_stdbyBV		(bADCCONstdby | (ADCCON_PRSCVL_bv << 6) | bADCCONch0)




#endif	// __S3C2440A_ADC_H
