/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440_tmr.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 PWM Timer registers
 *
 *****************************************************************************/

#ifndef __S3C2440A_TMR_H
#define __S3C2440A_TMR_H

#include "Garnet_sysdefs.h"

typedef struct  {
    UInt32 TCFG0;		// 0x51000000  Timer configuration      
    UInt32 TCFG1;		// 0x51000004  Timer configuration      
    UInt32 TCON;		// 0x51000008  Timer control            
    UInt32 TCNTB0;	// 0x5100000C  Timer count buffer 0     
    UInt32 TCMPB0;	// 0x51000010  Timer compare buffer 0   
    UInt32 TCNTO0;	// 0x51000014  Timer count observation 0
    UInt32 TCNTB1;	// 0x51000018  Timer count buffer 1     
    UInt32 TCMPB1;	// 0x5100001C  Timer compare buffer 1   
    UInt32 TCNTO1;	// 0x51000020  Timer count observation 1
    UInt32 TCNTB2;	// 0x51000024  Timer count buffer 2     
    UInt32 TCMPB2;	// 0x51000028  Timer compare buffer 2   
    UInt32 TCNTO2;	// 0x5100002C  Timer count observation 2
    UInt32 TCNTB3;	// 0x51000030  Timer count buffer 3     
    UInt32 TCMPB3;	// 0x51000034  Timer compare buffer 3   
    UInt32 TCNTO3;	// 0x51000038  Timer count observation 3
    UInt32 TCNTB4;	// 0x5100003C  Timer count buffer 4     
    UInt32 TCNTO4;	// 0x51000040  Timer count observation 4
    
} HwrS3C2440_TMR_RegType, *HwrS3C2440_TMR_RegPtr;


/*
	The S3C2440 has 5 16-bit timers, 4 of which have associated PWM output pins (TOUT0 - TOUT3)
	Timer 4 has no output pin
	
	In the PDA32 the 4 timer output pins are connected as follows:
	Timer 0 PWM output TOUT0 = BL_EN_PWM (backlight PWM, 1 = BL on)
	Timer 1 PWM output TOUT1 = BUZZER_PWM_EN (buzzer PWM)
	Timer 2 PWM output TOUT2 no connection
	Timer 3 PWM output TOUT3 = L3DATA (audio IC control) – the timer output function is not used on this pin

	Timer 3 is currently not used
	Timer 4 is currently used for HwrDelay() with 1 us resolution
	
	Timers 0 & 1 share 8-bit prescaler 0 from PCLK
	Timers 2, 3 & 4 share 8-bit prescaler 1 from PCLK
	Each timer has its own divider which can be set to 1/2, 1/4, 1/8 or 1/16 
	
	Since the timers share prescalers it is not possible to configure them independantly
	
	Examples of timer configuration
	With PCLK = 50 MHz
	For 1 us resolution use divider = 1/2 and prescaler = 25
	50 MHz/25/2 = 1 us/bit
	
	For 10 ms tick use divider = 1/4, prescaler = 125, count = 1000
	50 MHz/125/4/1000 = 10 ms
	
	For audio frequencies (beeps) we need PWM in the range 0 - 20kHz
	Set timer 1 frequency to 2.5MHz and use PWM periods in the range 125 -> 20kHz to 25,000 -> 100Hz
	
	For Backlight PWM we want the PWM frequency to be > 20kHz to avoid audible noise
	set timer 0 frequency to 5MHz and use PWM period = 100 -> 50kHz 
*/


/************************************************************************
 * Timer Configuration Register TCFG0 bits
 ************************************************************************/
#define bshftTCFG0prescale0		0		// 7:0, prescaler 0 for timers 0 & 1
#define bshftTCFG0prescale1		8		// 15:8, prescaler 1 for timers 2,3 & 4

#define bmaskTCFG0prescale0		(0xFF)
#define bmaskTCFG0prescale1		(0xFF << bshftTCFG0prescale1)

// prescalers
//#define TMR01PRESCALER		125-1			// 50 MHz/125 = 400kHz -> 200kHz, 100kHz, 50kHz, 25kHz
#define TMR01PRESCALER		(5-1)				// 50 MHz/5 = 10 MHz -> 5MHz, 2.5MHz, 1.25MHz, 625kHz
#define TMR234PRESCALER		(25-1)			// 50 MHz/25 = 2 MHz (1 us/tick with divider = 2)

#define bTCFG0init	((TMR234PRESCALER << bshftTCFG0prescale1) | TMR01PRESCALER)

/************************************************************************
 * Timer Configuration Register TCFG1 bits
 ************************************************************************/
// The divider options are the same for all 5 timers
#define TCFG1muxdiv2		0		// divider = 2
#define TCFG1muxdiv4		1		// divider = 4
#define TCFG1muxdiv8		2		// divider = 8
#define TCFG1muxdiv16		3		// divider = 16
#define TCFG1muxext			4		// external clock

// Bit shift values
#define bshftTCFG1mux0		0		// timer 0 MUX field
#define bshftTCFG1mux1		4		// timer 1 MUX field
#define bshftTCFG1mux2		8		// timer 2 MUX field
#define bshftTCFG1mux3		12	// timer 3 MUX field
#define bshftTCFG1mux4		16	// timer 4 MUX field
#define bshftTCFG1dma			20	// DMA channel

// Bit field masks
#define bmaskTCFG1mux0		(0xF << 0)		// timer 0 MUX field                      
#define bmaskTCFG1mux1		(0xF << 4)    // timer 1 MUX field                      
#define bmaskTCFG1mux2		(0xF << 8)    // timer 2 MUX field                      
#define bmaskTCFG1mux3		(0xF << 12)   // timer 3 MUX field                      
#define bmaskTCFG1mux4		(0xF << 16)   // timer 4 MUX field                      

#define bTCFG1init		(TCFG1muxdiv4 << 4)		// timer 1 divider = 1/4, all other dividers to 1/2, no DMA

#define TIMER1_FREQ				2500000		// 2.5MHz

#define	TIMER0_PWM_PRD		100				// 5mhZ / 100 = 50kHz


/************************************************************************
 * Timer Control Register TCON bits
 ************************************************************************/
// Timers 0,1,2,3 have associated output pin and may be inverted
// Timer 4 has no output pin and no invert control
// Each 16-bit timer counts down towards zero.
// On reaching zero a timer interrupt is generated and if autoreload is enabled
// the timer is automatically reloaded from the corresponding count buffer (TCNTBn)

#define bTCONt0start		BIT0			// timer 0 start/stop bit
#define bTCONt0update		BIT1			// timer 0 update bit
#define bTCONt0invert		BIT2			// timer 0 invert bit
#define bTCONt0auto			BIT3			// timer 0 auto reload bit
                      	
#define bTCONdze				BIT4			// dead zone enable bit

#define bTCONt1start		BIT8			// timer 1 start/stop bit
#define bTCONt1update		BIT9			// timer 1 update bit
#define bTCONt1invert		BIT10			// timer 1 invert bit
#define bTCONt1auto			BIT11			// timer 1 auto reload bit

#define bTCONt2start		BIT12			// timer 2 start/stop bit
#define bTCONt2update		BIT13			// timer 2 update bit
#define bTCONt2invert		BIT14			// timer 2 invert bit
#define bTCONt2auto			BIT15			// timer 2 auto reload bit

#define bTCONt3start		BIT16			// timer 3 start/stop bit
#define bTCONt3update		BIT17			// timer 3 update bit
#define bTCONt3invert		BIT18			// timer 3 invert bit
#define bTCONt3auto			BIT19			// timer 3 auto reload bit

#define bTCONt4start		BIT20			// timer 4 start/stop bit
#define bTCONt4update		BIT21			// timer 4 update bit
#define bTCONt4auto			BIT22			// timer 4 auto reload bit

// Bit field masks
#define bmaskTCONt0				(0xF << 0)		// timer 0 bits
#define bmaskTCONt1				(0xF << 8)		// timer 1 bits
#define bmaskTCONt2				(0xF << 12)		// timer 2 bits
#define bmaskTCONt3				(0xF << 16)		// timer 3 bits
#define bmaskTCONt4				(0x7 << 20)		// timer 4 bits


#endif	// __S3C2440A_TMR_H
