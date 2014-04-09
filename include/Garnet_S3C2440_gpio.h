/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: S3C2440_gpio.h
 *
 * Release: 
 *
 * Description:
 *		Hardware Equates for the Samsung S3C2440 GPIO registers
 *
 *****************************************************************************/

#ifndef __S3C2440A_GPIO_H
#define __S3C2440A_GPIO_H

/*
typedef struct {
	UInt32 GPACON; 		// 0x56000000 Port A control
	UInt32 GPADAT; 		// 0x56000004 Port A data   
	UInt32 PAD1[2];		// reserved
	
	UInt32 GPBCON; 		// 0x56000010 Port B control   
	UInt32 GPBDAT; 		// 0x56000014 Port B data      
	UInt32 GPBUP;  		// 0x56000018 Pull-up disable B
	UInt32 PAD2;		 	// reserved
	               		
	UInt32 GPCCON; 		// 0x56000020 Port C control  
	UInt32 GPCDAT; 		// 0x56000024 Port C data     
	UInt32 GPCUP;  		// 0x56000028 Pull-up disable C
	UInt32 PAD3;		 	// reserved
	               		    
	UInt32 GPDCON; 		// 0x56000030 Port D control  
	UInt32 GPDDAT; 		// 0x56000034 Port D data     
	UInt32 GPDUP;  		// 0x56000038 Pull-up disable D
	UInt32 PAD4;		 	// reserved
	               		    
	UInt32 GPECON; 		// 0x56000040 Port E control  
	UInt32 GPEDAT; 		// 0x56000044 Port E data     
	UInt32 GPEUP;  		// 0x56000048 Pull-up disable E
	UInt32 PAD5;		 	// reserved
	               		    
	UInt32 GPFCON; 		// 0x56000050 Port F control  
	UInt32 GPFDAT; 		// 0x56000054 Port F data     
	UInt32 GPFUP;  		// 0x56000058 Pull-up disable F
	UInt32 PAD6;		 	// reserved
	               		
	UInt32 GPGCON; 		// 0x56000060 Port G control  
	UInt32 GPGDAT; 		// 0x56000064 Port G data     
	UInt32 GPGUP;  		// 0x56000068 Pull-up disable G
	UInt32 PAD7;		 	// reserved
	               		
	UInt32 GPHCON; 		// 0x56000070 Port H control  
	UInt32 GPHDAT;    // 0x56000074 Port H data     
	UInt32 GPHUP;     // 0x56000078 Pull-up disable H
	UInt32 PAD8;		 	// reserved
	
	UInt32 MISCCR;  	// 0x56000080 Miscellaneous control
	UInt32 DCLKCON; 	// 0x56000084 DCLK0/1 control      
	                	
	UInt32 EXTINT0; 	// 0x56000088 external interrupt control reg 0
	UInt32 EXTINT1; 	// 0x5600008C external interrupt control reg 1
	UInt32 EXTINT2; 	// 0x56000090 external interrupt control reg 2
	                	    
	UInt32 EINTFLT0;	// 0x56000094 reserved
	UInt32 EINTFLT1;	// 0x56000098 reserved
	UInt32 EINTFLT2;	// 0x5600009C external interrupt filter reg 2
	UInt32 EINTFLT3;	// 0x560000A0 external interrupt filter reg 3
	                	    
	UInt32 EINTMASK;	// 0x560000A4 external interrupt mask reg
	UInt32 EINTPEND;	// 0x560000A8 external interrupt pending reg
	                	    
	UInt32 GSTATUS0;	// 0x560000AC external pin status
	UInt32 GSTATUS1;	// 0x560000B0 chip ID
	UInt32 GSTATUS2;	// 0x560000B4 reset status
	UInt32 GSTATUS3;	// 0x560000B8 inform register
	UInt32 GSTATUS4;	// 0x560000BC inform register
	                      
	UInt32 FLTOUT;		// there is no register here
	UInt32 DSC0;      //  
	UInt32 DSC1;      //  
	UInt32 MSLCON;    // 0x560000CC Memory sleep control register
	                      
	UInt32 GPJCON;		// 0x560000D0 Port J control  
	UInt32 GPJDAT;    // 0x560000D4 Port J data     
	UInt32 GPJUP;     // 0x560000D8 Pull-up disable J
	UInt32 PDA9;		 	// reserved
	
} HwrS3C2440_GPIO_RegType, *HwrS3C2440_GPIO_RegPtr;  
*/

//------------------------------------------------------------------------------

// Individual Port bit names
#define bGPCON_Input  		0x00		// Input bits
#define bGPCON_Output  		0x01		// Output bits
#define bGPCON_Dedicated  0x10		// Dedicated bits
#define bGPCON_Reserved  	0x11		// Reserve bits


/************************************************************************
 * PortA bits
 ************************************************************************/ 
// Port A
//Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16     GPA15   GPA14 GPA13 GPA12 GPA11  GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1   GPA0
//Signal : nFCE nRSTOUT nFRE  nFWE  ALE   CLE   AUDIO_ENn x 			nGCS3 nGCS2 nGCS1 ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0

// GPADAT bit masks
#define bGPADAT_ADDR0                		BIT0      // Address line
#define bGPADAT_ADDR16               		BIT1      // Address line
#define bGPADAT_ADDR17               		BIT2      // Address line
#define bGPADAT_ADDR18               		BIT3      // Address line
#define bGPADAT_ADDR19               		BIT4      // Address line
#define bGPADAT_ADDR20               		BIT5      // Address line
#define bGPADAT_ADDR21               		BIT6      // Address line
#define bGPADAT_ADDR22               		BIT7      // Address line
#define bGPADAT_ADDR23               		BIT8      // Address line
#define bGPADAT_ADDR24               		BIT9      // Address line
#define bGPADAT_ADDR25               		BIT10     // Address line
#define bGPADAT_ADDR26               		BIT11     // Address line
#define bGPADAT_nGCS1                		BIT12     // Chip select
#define bGPADAT_nGCS2                		BIT13     // Chip select
#define bGPADAT_nGCS3                		BIT14     // Chip select
                                     		
#define bGPADAT_AUDIO_ENn            		BIT16     // Audio enable
#define bGPADAT_CLE                  		BIT17     // CLE
#define bGPADAT_ALE                  		BIT18     // ALE
#define bGPADAT_nFWE                 		BIT19     // nFWE
#define bGPADAT_nFRE                 		BIT20     // nFRE
#define bGPADAT_nRSTOUT              		BIT21     // nRSTOUT
#define bGPADAT_nFCE                 		BIT22     // nFCE

/************************************************************************
 * PortB bits
 ************************************************************************/ 
// Port B
// Ports  : GPB10        GPB9         GPB8    GPB7         GPB6         GPB5     GPB4    GPB3   GPB2  GPB1          GPB0
// Signal : CHG_PWRGD  	 CHG_FAULTn   L3MODE  CHG_STATn    PSU_PWRDN    x  			 L3CLOCK L3DATA x 		BUZZER_PWM_EN BL_EN_PWM
 
// GPBDAT bit masks
#define bGPBDAT_BL_EN_PWM            		BIT0      // Backlight enable
#define bGPBDAT_BUZZER_PWM_EN        		BIT1      // Buzzer enable
                                     		
#define bGPBDAT_L3DATA               		BIT3      // Audio IC L3 data
#define bGPBDAT_L3CLOCK              		BIT4      // Audio IC L3 clock
                                     		
#define bGPBDAT_PSU_PWRDN            		BIT6      // PSU power down
#define bGPBDAT_CHG_STATn            		BIT7      // Charger status
#define bGPBDAT_L3MODE               		BIT8      // Audio IC L3 mode
#define bGPBDAT_CHG_FAULTn           		BIT9      // Charger fault
#define bGPBDAT_CHG_PWRGD            		BIT10     // Charger power good
                                 	
/************************************************************************
 * PortC bits
 ************************************************************************/ 
// Port C
// Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8 GPC7  	  GPC6   			GPC5   			GPC4       GPC3        GPC2        GPC1      GPC0
// Signal : VD7   VD6   VD5   VD4   VD3   x   	x 	 x 	  LCD_RESET LCD_SPI_CLK LCD_SPI_SDO LCD_VM_EN  LCD_VSYNC   LCD_HYSNC   LCD_PCLK  LCD_SPI_CS
            	
// GPCDAT bit masks            	
#define bGPCDAT_LCD_SPI_CSn        			BIT0      // LCD SPI chip select
#define bGPCDAT_LCD_PCLK           			BIT1      // LCD PCLK
#define bGPCDAT_LCD_HYSNC          			BIT2      // LCD HYSNC
#define bGPCDAT_LCD_VSYNC          			BIT3      // LCD VSYNC
#define bGPCDAT_LCD_VM_EN          			BIT4      // LCD VM EN
#define bGPCDAT_LCD_SPI_SDO        			BIT5      // LCD SPI data
#define bGPCDAT_LCD_SPI_CLK        			BIT6      // LCD SPI clock
#define bGPCDAT_LCD_RESETn         			BIT7      // LCD Reset

#define bGPCDAT_VD3                			BIT11     // LCD VD3
#define bGPCDAT_VD4                			BIT12     // LCD VD4
#define bGPCDAT_VD5                			BIT13     // LCD VD5
#define bGPCDAT_VD6                			BIT14     // LCD VD6
#define bGPCDAT_VD7                			BIT15     // LCD VD7


// Bit shift values
#define bsGPCDAT_LCD_SPI_CS         		0      		// LCD SPI chip select      
#define bsGPCDAT_LCD_PCLK           		1         // LCD PCLK                 
#define bsGPCDAT_LCD_HYSNC          		2         // LCD HYSNC                
#define bsGPCDAT_LCD_VSYNC          		3         // LCD VSYNC                
#define bsGPCDAT_LCD_VM_EN          		4         // LCD VM EN                
#define bsGPCDAT_LCD_SPI_SDO        		5         // LCD SPI data             
#define bsGPCDAT_LCD_SPI_CLK        		6         // LCD SPI clock            
#define bsGPCDAT_LCD_RESETn         		7         // LCD Reset                
                       
#define bsGPCDAT_VD3                		11        // LCD VD3                  
#define bsGPCDAT_VD4                		12        // LCD VD4                  
#define bsGPCDAT_VD5                		13        // LCD VD5                  
#define bsGPCDAT_VD6                		14        // LCD VD6                  
#define bsGPCDAT_VD7                		15        // LCD VD7                  


// definitions for LCD controller initialisation                	    
#define bfGPCCON_LCDctrl 	0xFFC0FFFF
#define	bGPCCON_LCDinit	 0xAA8056A9 

// pullups disabled on all LCD control pins                     	    
#define	bGPCUP_LCDctrl															\
	(													                      	\
	(bGPCDAT_LCD_SPI_CSn) 		                    |	\
	(bGPCDAT_LCD_PCLK) 			                      |	\
	(bGPCDAT_LCD_HYSNC) 			                    |	\
	(bGPCDAT_LCD_VSYNC)     	                    | \
	(bGPCDAT_LCD_VM_EN)     	                    | \
	(bGPCDAT_LCD_SPI_SDO) 		                    | \
	(bGPCDAT_LCD_SPI_CLK)   	                    | \
	(bGPCDAT_LCD_RESETn) 			                    | \
	(bGPCDAT_VD3) 					                      | \
	(bGPCDAT_VD4)   	                            | \
	(bGPCDAT_VD5) 	                              | \
	(bGPCDAT_VD6)	    	                          | \
	(bGPCDAT_VD7)                                 	\
	)           

// Data lines for SPI control are high
#define	bGPCDAT_LCDctrl														\
	(													                     	\
	(bGPCDAT_LCD_SPI_CSn) 		                    |	\
	(bGPCDAT_LCD_SPI_SDO) 		                    | \
	(bGPCDAT_LCD_SPI_CLK)   	                    | \
	(bGPCDAT_LCD_RESETn) 			                    	\
	)           


                        	         	
/************************************************************************
 * PortD bits
 ************************************************************************/ 
// Port D
// Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 	GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 	GPD0
// Signal : VD23  VD22  VD21  VD20  VD19  x  		x 		x 		VD15 VD14 VD13 VD12 VD11 VD10 x 	 	x

// GPDDAT bit masks      	         	

#define bGPDDAT_VD10                            BIT2      // LCD VD10
#define bGPDDAT_VD11                            BIT3      // LCD VD11
#define bGPDDAT_VD12                            BIT4      // LCD VD12
#define bGPDDAT_VD13                            BIT5      // LCD VD13
#define bGPDDAT_VD14                            BIT6      // LCD VD14
#define bGPDDAT_VD15                            BIT7      // LCD VD15
 
#define bGPDDAT_VD19                            BIT11     // LCD VD19
#define bGPDDAT_VD20                            BIT12     // LCD VD20
#define bGPDDAT_VD21                            BIT13     // LCD VD21
#define bGPDDAT_VD22                            BIT14     // LCD VD22
#define bGPDDAT_VD23                            BIT15     // LCD VD23
           
// definitions for LCD controller initialisation                	    
#define bfGPDCON_LCDctrl 	0xFFC0FFF0

#define	bGPDCON_LCDinit		0xAA80AAA0

// pullups disabled on all LCD control pins                     	    
#define	bGPDUP_LCDctrl	\
	(								\
	bGPDDAT_VD10 		|\
	bGPDDAT_VD11 		|\
	bGPDDAT_VD12 		|\
	bGPDDAT_VD13 		|\
	bGPDDAT_VD14		|\
	bGPDDAT_VD15		|\
	bGPDDAT_VD19		|\
	bGPDDAT_VD20		|\
	bGPDDAT_VD21		|\
	bGPDDAT_VD22		|\
	bGPDDAT_VD23		|\
	bGPCDAT_VD7			\
	)   	           
                      	
/************************************************************************
 * PortE bits
 ************************************************************************/ 
// Ports  : GPE15  GPE14  GPE13         GPE12         GPE11            GPE10   GPE9    GPE8    GPE7    GPE6  GPE5  GPE4   GPE3   GPE2  GPE1    GPE0
// Signal : IICSDA IICSCL TEST_SPI_CLK  TEST_SPI_MOSI TEST_SPI_MISO    SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO I2SSDI CDCLK I2SSCLK I2SLRCK
 
// GPEDAT bit masks
#define bGPEDAT_I2SLRCK                         BIT0      // Audio I2SLRCK
#define bGPEDAT_I2SSCLK                         BIT1      // Audio I2SSCLK
#define bGPEDAT_CDCLK                           BIT2      // Audio CDCLK
#define bGPEDAT_I2SSDI                          BIT3      // Audio I2SSDI
#define bGPEDAT_I2SSDO                          BIT4      // Audio I2SSDO
#define bGPEDAT_SDCLK                           BIT5      // SDMMC SDCLK
#define bGPEDAT_SDCMD                           BIT6      // SDMMC SDCMD
#define bGPEDAT_SDDATA0                         BIT7      // SDMMC SDDATA0
#define bGPEDAT_SDDATA1                         BIT8      // SDMMC SDDATA1
#define bGPEDAT_SDDATA2                         BIT9      // SDMMC SDDATA2
#define bGPEDAT_SDDATA3                         BIT10     // SDMMC SDDATA3
#define bGPEDAT_TEST_SPI_MISO                   BIT11     // TEST SPI MISO
#define bGPEDAT_TEST_SPI_MOSI                   BIT12     // TEST SPI MOSI
#define bGPEDAT_TEST_SPI_CLK                    BIT13     // TEST SPI CLK
#define bGPEDAT_IICSCL                          BIT14     // I2C clock
#define bGPEDAT_IICSDA                          BIT15     // I2C data
                        
#define bGPEDAT_I2Cbus			(bGPEDAT_IICSCL | bGPEDAT_IICSDA)									// I2C bus, SDA & SCL
                          	
#define bfGPECON_I2Cbus			(BIT31 | BIT30 | BIT29 | BIT28)	// I2C bus control, SDA & SCL field
#define bGPECON_I2Cbus			(BIT30 | BIT28)									// I2C bus control, SDA & SCL = outputs
                        	         	
/************************************************************************
 * PortF bits
 ************************************************************************/ 
// Ports  : GPF7        	GPF6        GPF5  GPF4  GPF3        GPF2       GPF1          	GPF0
// Signal : KP_nRightKey 	KP_nLeftKey	x			x			KP_nDownKey	KP_nUpKey  KP_nBlightKey 	KP_PowerKey
// GPFDAT bit masks      	         	
#define bGPFDAT_KP_PowerKey     			BIT0      // Keypad KP_PowerKey
#define bGPFDAT_KP_nBlightKey   			BIT1      // Keypad KP_nBlightKey
#define bGPFDAT_KP_nUpKey       			BIT2      // Keypad KP_nUpKey
#define bGPFDAT_KP_nDownKey     			BIT3      // Keypad KP_nDownKey

#define bGPFDAT_KP_nLeftKey     			BIT6      // Keypad KP_nLeftKey
#define bGPFDAT_KP_nRightKey    			BIT7      // Keypad KP_nRightKey

// GPFCON interrupt configuration masks
#define	bGPFCONeint0		(2 << 0)		// EINT0 = Power key interrupt
#define	bGPFCONeint1		(2 << 2)		// EINT1 = Backlight key interrupt
#define	bGPFCONeint2		(2 << 4)		// EINT2 = Up key interrupt
#define	bGPFCONeint3		(2 << 6)		// EINT3 = Down key interrupt

#define	bGPFCONeint6		(2 << 12)		// EINT6 = Left key interrupt
#define	bGPFCONeint7		(2 << 14)		// EINT7 = Right key interrupt

// GPFCON bit field masks
#define	bfGPFCONeint0		(3 << 0)		// EINT0 = Power key interrupt
#define	bfGPFCONeint1		(3 << 2)		// EINT1 = Backlight key interrupt
#define	bfGPFCONeint2		(3 << 4)		// EINT2 = Up key interrupt
#define	bfGPFCONeint3		(3 << 6)		// EINT3 = Down key interrupt

#define	bfGPFCONeint6		(3 << 12)		// EINT6 = Left key interrupt
#define	bfGPFCONeint7		(3 << 14)		// EINT7 = Right key interrupt

// Bit mask to configure all keyboard interrupt pins
#define	bGPFCONeintKP (bGPFCONeint7 | bGPFCONeint6 | bGPFCONeint3 | bGPFCONeint2 | bGPFCONeint1 | bGPFCONeint0)

// Bit field mask for all keyboard interrupt configuration bits
// Clear these bits to configure the keys as input pins
#define	bfGPFCONinputKP (bfGPFCONeint7 | bfGPFCONeint6 | bfGPFCONeint3 | bfGPFCONeint2 | bfGPFCONeint1 | bfGPFCONeint0)

           	         	
                                 	
/************************************************************************
 * PortG bits
 ************************************************************************/
// Ports  : GPG15 GPG14 GPG13 GPG12   GPG11	GPG10 GPG9  GPG8    GPG7  GPG6  GPG5       		GPG4    GPG3           			GPG2        GPG1     		GPG0
// Signal : x  		x  		x  		USB_ID	x			x			x			SD_CDn  x			x			TEST_SPI_CSn	LCD_ON  USB_CRADLE_HOST_ENn	Test_SPISSn	KP_ResetKey	KP_nCentreKey

// GPGDAT bit masks
#define bGPGDAT_KP_nCentreKey           	BIT0      // Keypad KP_nCentreKey
#define bGPGDAT_KP_ResetKey             	BIT1      // Keypad KP_ResetKey
#define bGPGDAT_Test_SPISSn             	BIT2      // TEST SPI slave select
#define bGPGDAT_USB_CRADLE_HOST_ENn     	BIT3      // USB CRADLE HOST ENn
#define bGPGDAT_LCD_ON                  	BIT4      // LCD ON
#define bGPGDAT_TEST_SPI_CSn            	BIT5      // TEST SPI chip select

#define bGPGDAT_SD_CDn                		BIT8      // SDMMC SD_CDn

#define bGPGDAT_USB_ID               			BIT12     // USB ID input line


// Bit shift values
#define bsGPGDAT_KP_nCentreKey         		0       	// Keypad KP_nCentreKey       
#define bsGPGDAT_KP_ResetKey           		1         // Keypad KP_ResetKey         
#define bsGPGDAT_Test_SPISSn           		2         // TEST SPI slave select      
#define bsGPGDAT_USB_CRADLE_HOST_ENn   		3         // USB CRADLE HOST ENn        
#define bsGPGDAT_LCD_ON                		4         // LCD ON                     
#define bsGPGDAT_TEST_SPI_CSn          		5         // TEST SPI chip select       
                           
#define bsGPGDAT_SD_CDn                		8         // SDMMC SD_CDn               
                          
#define bsGPGDAT_USB_ID                		12        // USB ID input line          
                          


// GPGCON interrupt configuration masks
#define	bGPGCONeint8		(2 << 0)		// EINT8 = Centre key interrupt
#define	bGPGCONeint9		(2 << 2)		// EINT9 = Reset key interrupt

// GPGCON bit field masks
#define	bfGPGCONeint8		(3 << 0)		// EINT8 = Centre key interrupt
#define	bfGPGCONeint9		(3 << 2)		// EINT9 = Reset key interrupt

// Bit mask to configure Centre key and reset key as interrupt pins
#define	bGPGCONeintKP (bGPGCONeint9 | bGPGCONeint8)

// Clear these bits to configure Centre key and reset key as input pins
#define	bfGPGCONinputKP (bfGPGCONeint9 | bfGPGCONeint8)


/************************************************************************
 * PortH bits
 ************************************************************************/ 
// Ports  : GPH10 GPH9  GPH8  GPH7      GPH6        GPH5  GPH4  GPH3  GPH2  GPH1  GPH0
// Signal : x  		x			x			IrDA_RXD  IrDA_TXD    RXD1	TXD1	RXD0	TXD0	x   	x

// GPHDAT bit masks

#define bGPHDAT_TXD0          		BIT2      // UART TXD0
#define bGPHDAT_RXD0          		BIT3      // UART RXD0
#define bGPHDAT_TXD1          		BIT4      // UART TXD1
#define bGPHDAT_RXD1          		BIT5      // UART RXD1
#define bGPHDAT_IrDA_TXD      		BIT6      // UART IrDA_TXD
#define bGPHDAT_IrDA_RXD      		BIT7      // UART IrDA_RXD


/************************************************************************
 * PortJ bits
 ************************************************************************/ 
// Ports  : GPJ12 GPJ11          	GPJ10	  GPJ9  GPJ8  	GPJ7      	GPJ6    GPJ5          GPJ4        GPJ3        GPJ2        GPJ1	GPJ0
// Signal : x   	Audio_HPDetect	CHG_ENn	SD_WP	SD_ENn  VIBMTR_ENn	IrDA_SD	RemoteLED_TX	ADC_ENABLE	CHG_1000_EN	CHG_500_EN	x 		x

// GPJDAT bit masks

#define bGPJDAT_CHG_500_EN      	BIT2      // Charger 500mA enable
#define bGPJDAT_CHG_1000_EN     	BIT3      // Charger 1000mA enable
#define bGPJDAT_ADC_ENABLE      	BIT4      // ADC enable
#define bGPJDAT_RemoteLED_TX    	BIT5      // IrLED TX
#define bGPJDAT_IrDA_SD         	BIT6      // IrDA shutdown
#define bGPJDAT_VIBMTR_ENn      	BIT7      // Vibrator motor enable
#define bGPJDAT_SD_ENn          	BIT8      // SDMMC SD_ENn
#define bGPJDAT_SD_WP           	BIT9      // SDMMC SD_WP
#define bGPJDAT_CHG_ENn         	BIT10     // Charger enable
#define bGPJDAT_Audio_HPDetect  	BIT11     // Audio headphone detect


/************************************************************************
 * General Status Register0 GSTATUS0 bit
 ************************************************************************/ 

#define bGST0_BATT_FLT			BIT0		// status of BATT_FLT pin in GSTATUS0
      
/************************************************************************
 * General Status Register2 GSTATUS2 bits
 ************************************************************************/ 
                    	      	
#define bGST2_PWRST					BIT0		// power on reset
#define bGST2_SLEEPRST			BIT1		// wakeup reset 9from sleep)
#define bGST2_WDTRST				BIT2		// watchdog reset
                        		
#define bGST2_ALLRST				BIT0 | BIT1 | BIT2		// mask to clear all reset reasons

/************************************************************************
 * Miscellaneous Control Register MISCCR bits
 ************************************************************************/
 
#define	MISCCR_BATTFLT_RST		0						// nBATT_FLT = 0 holds CPU in reset
#define	MISCCR_BATTFLT_WUINT	4						// nBATT_FLT = 0 wakeup from sleep, int when awake
#define	MISCCR_BATTFLT_NOWU		6						// nBATT_FLT = 0 prevents wakeup, no affect if awake
#define	MISCCR_BATTFLT_DIS		7						// nBATT_FLT function disabled

#define	MISCCR_CLKSEL0init		2		// reset value	
#define	MISCCR_CLKSEL1init		0		// reset value	

// Bit shift value 
#define	bsMISCCRclksel0			4			// CLKSEL0 [6:4] bits
#define	bsMISCCRclksel1			8			// CLKSEL1 [10:8] bits
#define	bsMISCCRbattfunc		20		// BATT_FUNC [22:20] bits

// Bit field mask
#define	bfMISCCRclksel0			(7 << 4)		// CLKSEL0
#define	bfMISCCRclksel1			(7 << 8)		// CLKSEL1
#define	bfMISCCRbattfunc		(7 << 20)		// BATT_FUNC

// Bit masks
#define	bMISCCRspucr0				BIT0		// SPUCR0 data[31:16] pullup disable
#define	bMISCCRspucr1				BIT1		// SPUCR1 data[15:0] pullup disable
#define	bMISCCRusbHost			BIT3		// SEL_USBPAD 0: USB1 = device, 1: USB1 = host
#define	bMISCCRsuspnd0			BIT12		// SEL_SUSPND0 USB0 suspend
#define	bMISCCRsuspnd1			BIT13		// SEL_SUSPND0 USB1 suspend
#define	bMISCCRrstcon				BIT16		// nRSTCON nRSTOUT level
#define	bMISCCRnOEsclk0			BIT17		// nEN_SCLK0 0: SCLK0 = 0, 1: SCLK0 = SCLK
#define	bMISCCRnOEsclk1			BIT18		// nEN_SCLK1 0: SCLK1 = 0, 1: SCLK1 = SCLK
#define	bMISCCRoffrefresh		BIT19		// OFFREFRESH 1: self refresh retained after sleep, 0: disabled

#define	bMISCCR_BATTFLT_RST		(MISCCR_BATTFLT_RST << bsMISCCRbattfunc)
#define	bMISCCR_BATTFLT_WUINT	(MISCCR_BATTFLT_WUINT << bsMISCCRbattfunc)
#define	bMISCCR_BATTFLT_NOWU	(MISCCR_BATTFLT_NOWU << bsMISCCRbattfunc)
#define	bMISCCR_BATTFLT_DIS		(MISCCR_BATTFLT_DIS << bsMISCCRbattfunc)

// Overall values (CLKSEL0 & 1 are don't care as these pins are configured as gpio)
#define	bMISCCR_init		(bMISCCR_BATTFLT_RST | bMISCCRrstcon)																			// 
#define	bMISCCR_sleep		(bMISCCR_BATTFLT_NOWU | bMISCCRrstcon | bMISCCRsuspnd1 | bMISCCRsuspnd0 )	// no wakeup, pullups on data bus
#define	bMISCCR_wake		(MISCCR_BATTFLT_WUINT | bMISCCRrstcon)																		// BATT_FLT int

/************************************************************************
 * Memory Sleep Control Register MSLCON bits
 ************************************************************************/
// Bit masks
#define	bMSLCONgcs7				BIT0		// PSC_GCS7 sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONgcs6				BIT1		// PSC_GCS6 sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONgcs51			BIT2		// PSC_GCS[5:1] sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONgcs0				BIT3		// PSC_GCS0 sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONwe					BIT4		// PSC_WE sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONoe					BIT5		// PSC_OE sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONdqm				BIT6		// PSC_DQM sleep 0: inactive 1: Hi-Z
#define	bMSLCONsdr				BIT7		// PSC_SDR sleep 0: inactive(1) 1: Hi-Z
#define	bMSLCONnf					BIT8		// PSC_NF sleep 0: inactive 1: Hi-Z
#define	bMSLCONrnb				BIT9		// PSC_RnB sleep 0: input 1: output low
#define	bMSLCONwait				BIT10		// PSC_WAIT sleep 0: input 1: output low
#define	bMSLCONdata				BIT11		// PSC_DATA sleep 0: Hi-Z 1: output low (need pullups enabled if this is Hi-Z)

// MSLCON.11 = 0 (below) sets data bus to Hi-Z so NEED pullups
#define	bMSLCON_init		0		// data = Hi-Z in sleep so need pullups 9see MISCCR reg)

/************************************************************************
 * External Interrupt Control Register EXTINT[2:0] bits
 ************************************************************************/
 
// Interrupt configuration for EINT[23:0]
#define	EINTlowlvl		0		// Low level trigger
#define	EINThighlvl		1		// High level trigger
#define	EINTfalling		2		// Falling edge trigger
#define	EINTrising		4		// Rising edge trigger
#define	EINTboth			6		// Both edge trigger

// EXTINT0 bit shift values - use for correct alignment of fields
#define	bsEXTINT0eint0		0
#define	bsEXTINT0eint1		4
#define	bsEXTINT0eint2		8
#define	bsEXTINT0eint3		12
#define	bsEXTINT0eint4		16
#define	bsEXTINT0eint5		20
#define	bsEXTINT0eint6		24
#define	bsEXTINT0eint7		28

// EXTINT0 bit field masks - use inverted to clear bits
#define	bfEXTINT0eint0		(7 << bsEXTINT0eint0)
#define	bfEXTINT0eint1		(7 << bsEXTINT0eint1)
#define	bfEXTINT0eint2		(7 << bsEXTINT0eint2)
#define	bfEXTINT0eint3		(7 << bsEXTINT0eint3)
#define	bfEXTINT0eint4		(7 << bsEXTINT0eint4)
#define	bfEXTINT0eint5		(7 << bsEXTINT0eint5)
#define	bfEXTINT0eint6		(7 << bsEXTINT0eint6)
#define	bfEXTINT0eint7		(7 << bsEXTINT0eint7)

// EXTINT1 bit shift values - use for correct alignment of fields
#define	bsEXTINT1eint8		0
#define	bsEXTINT1eint9		4
#define	bsEXTINT1eint10		8
#define	bsEXTINT1eint11		12
#define	bsEXTINT1eint12		16
#define	bsEXTINT1eint13		20
#define	bsEXTINT1eint14		24
#define	bsEXTINT1eint15		28

// EXTINT1 bit field masks - use inverted to clear bits
#define	bfEXTINT1eint8		(0x0F << bsEXTINT0eint8)
#define	bfEXTINT1eint9		(0x0F << bsEXTINT0eint9)
#define	bfEXTINT1eint10		(0x0F << bsEXTINT0eint10)
#define	bfEXTINT1eint11		(0x0F << bsEXTINT0eint11)
#define	bfEXTINT1eint12		(0x0F << bsEXTINT0eint12)
#define	bfEXTINT1eint13		(0x0F << bsEXTINT0eint13)
#define	bfEXTINT1eint14		(0x0F << bsEXTINT0eint14)
#define	bfEXTINT1eint15		(0x0F << bsEXTINT0eint15)

// EXTINT1 bit masks to enable interrupt filter
#define	bEXTINT1flten8		BIT3
#define	bEXTINT1flten9		BIT7
#define	bEXTINT1flten10		BIT11
#define	bEXTINT1flten11		BIT15
#define	bEXTINT1flten12		BIT19
#define	bEXTINT1flten13		BIT23
#define	bEXTINT1flten14		BIT27
#define	bEXTINT1flten15		BIT31

// EXTINT2 bit shift values - use for correct alignment of fields
#define	bsEXTINT2eint16		0
#define	bsEXTINT2eint17		4
#define	bsEXTINT2eint18		8
#define	bsEXTINT2eint19		12
#define	bsEXTINT2eint20		16
#define	bsEXTINT2eint21		20
#define	bsEXTINT2eint22		24
#define	bsEXTINT2eint23		28

// EXTINT2 bit field masks - use inverted to clear bits
#define	bfEXTINT2eint16		(0x0F << bsEXTINT0eint16)
#define	bfEXTINT2eint17		(0x0F << bsEXTINT0eint17)
#define	bfEXTINT2eint18		(0x0F << bsEXTINT0eint18)
#define	bfEXTINT2eint19		(0x0F << bsEXTINT0eint19)
#define	bfEXTINT2eint20		(0x0F << bsEXTINT0eint20)
#define	bfEXTINT2eint21		(0x0F << bsEXTINT0eint21)
#define	bfEXTINT2eint22		(0x0F << bsEXTINT0eint22)
#define	bfEXTINT2eint23		(0x0F << bsEXTINT0eint23)

// EXTINT2 bit masks to enable interrupt filter
#define	bEXTINT2flten16		BIT3
#define	bEXTINT2flten17		BIT7
#define	bEXTINT2flten18		BIT11
#define	bEXTINT2flten19		BIT15
#define	bEXTINT2flten20		BIT19
#define	bEXTINT2flten21		BIT23
#define	bEXTINT2flten22		BIT27
#define	bEXTINT2flten23		BIT31


/************************************************************************
 * External Interrupt Mask and Pending Registers EINTMASK and EINTPEND
 ************************************************************************/

#define	bEINT4		BIT4
#define	bEINT5		BIT5
#define	bEINT6		BIT6
#define	bEINT7		BIT7
#define	bEINT8		BIT8	
#define	bEINT9		BIT9	
#define	bEINT10 	BIT10 
#define	bEINT11 	BIT11 
#define	bEINT12 	BIT12 
#define	bEINT13 	BIT13 
#define	bEINT14 	BIT14 
#define	bEINT15 	BIT15 
#define	bEINT16		BIT16
#define	bEINT17		BIT17
#define	bEINT18		BIT18
#define	bEINT19		BIT19
#define	bEINT20		BIT20
#define	bEINT21		BIT21
#define	bEINT22		BIT22
#define	bEINT23		BIT23

// Mask for all individual key interrupts (in EINTMASK & EINTPEND)
#define	bEINT6789	(bEINT6 | bEINT7 | bEINT8 | bEINT9)


#endif	// __S3C2440A_GPIO_H
