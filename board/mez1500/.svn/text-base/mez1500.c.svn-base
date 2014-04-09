/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 * 
 * (C) Copyright 2009
 * Michel Pollet <buserror@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <s3c2440.h>
#include <video_fb.h>

DECLARE_GLOBAL_DATA_PTR;

/* FCLK = 405 MHz, HCLK = 101 MHz, PCLK = 50 MHz, UCLK = 48 MHz */
#define CLKDIVN_VAL	5
#define M_MDIV		0x7f
#define M_PDIV		0x2
#define M_SDIV		0x1

#define U_M_MDIV	0x38
#define U_M_PDIV	0x2
#define U_M_SDIV	0x2

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
	  "subs %0, %1, #1\n"
	  "bne 1b":"=r" (loops):"0" (loops));
}

void HALDelay(uint32_t microseconds)
{
	do 
		delay(100);
	while (microseconds--);
}

/*
 * Miscellaneous platform dependent initialisations
 */



int board_init (void)
{
	S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();

	/* to reduce PLL lock time, adjust the LOCKTIME register */
	clk_power->LOCKTIME = 0xFFFFFF;
	clk_power->CLKDIVN = CLKDIVN_VAL;

	/* configure UPLL */
	clk_power->UPLLCON = ((U_M_MDIV << 12) + (U_M_PDIV << 4) + U_M_SDIV);
	/* some delay between MPLL and UPLL */
	delay (10);
	/* configure MPLL */
	clk_power->MPLLCON = ((M_MDIV << 12) + (M_PDIV << 4) + M_SDIV);

	/* some delay between MPLL and UPLL */
	delay (8000);

	/* Port A */
	/* Ports  : GPA22 GPA21   GPA20 GPA19 GPA18 GPA17 GPA16     GPA15 				GPA14       GPA13   GPA12   GPA11  GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1   GPA0		*/
	/* Signal : nFCE  nRSTOUT nFRE  nFWE  ALE   CLE   AUDIO_ENn KP_MATRIX_CSn VIBMTR_ENn  CHG_ENn L3MODE  ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0  */
	/* Binary : 1     1       1     1     1     1     0         1     				0           0       0       1      1      1      1      1      1      1      1      1      1      1      1    	*/
	/* Data 	: 0     0       0     0     0     0     1         1     				1           0       0       0      0      0      0      0      0      0      0      0      0      0      0    	*/
    gpio->GPADAT = 0x0001C000;
    gpio->GPACON = 0x7E8FFF;


	/* Port B */
	/* Ports  : GPB10 			GPB9       GPB8         GPB7        GPB6      GPB5 			    GPB4    GPB3   GPB2 						GPB1          GPB0       */
  /* Signal : CHG_PWR_GDn CHG_STAT2n 5V0_SMPS_ENn CHG_STAT1n  PSU_PWRDN WLESS_5V0_ENn L3CLOCK L3DATA MZIO_SPI_DRN_PWM BUZZER_PWM_EN BL_EN_PWM  */
	/* Setting: IN    			IN         OUT          IN          OUT       OUT  			    OUT     OUT    OUT  						OUT           OUT        */
	/* Binary : 00    			00         01           00        	01        01   			    01      01     01   						01            01         */
	/* PU_OFF : 0     			0          1            0         	1         1    			    1       1      1    						1             1          */
	/* DATA   : 0     			0          0            0         	0         0    			    0       0      0    						0             1          */
    gpio->GPBDAT = 0x001;
    gpio->GPBCON = 0x11555;
    gpio->GPBUP  = 0x17F;


	/* Port C */
	/* Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9      GPC8      GPC7      GPC6        GPC5    GPC4    GPC3      GPC2      GPC1     GPC0        */
	/* Signal : VD7   VD6   VD5   VD4   VD3   VD2   5V_CR_ENn 5V_MZ_ENn BT_RESETn WIFI_RESETn WIFI_CS WIFI_EN LCD_VSYNC LCD_HYSNC LCD_PCLK Wless_BT_EN */
	/* Setting: OUT   OUT   OUT   OUT   OUT   OUT   OUT       OUT       IN        IN          OUT     OUT     OUT       OUT       OUT      OUT         */
	/* Binary : 01    01    01    01    01    01    01        01        00        00          01      01      01        01        01       01          */
	/* PU_OFF : 1     1     1     1     1     1     1         1         1         1           1       1       1         1         1        1 	         */
	/* DATA   : 0     0     0     0     0     0     0         0         1         1           0       0       0         0         0        0 	         */
    gpio->GPCDAT = 0x0C0;
    gpio->GPCCON = 0x55550555;
    gpio->GPCUP  = 0xFFFF;


	/* Port D */
	/* Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9     GPD8         GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1         GPD0       */
	/* Signal : VD23  VD22  VD21  VD20  VD19  VD18  RS232_EN MZIO_IrDAENn VD15 VD14 VD13 VD12 VD11 VD10 MZIO_MOD_PWR MZIO_RESET */
	/* Setting: OUT   OUT   OUT   OUT   OUT   OUT   OUT      OUT          OUT  OUT  OUT  OUT  OUT  OUT  OUT          OUT        */
	/* Binary : 01    01    01    01    01    01    01       01           01   01   01   01   01   01   01           01         */
	/* PU_OFF : 1     1     1     1     1     1     1        1            1    1    1    1    1    1    1            1          */
	/* DATA   : 0     0     0     0     0     0     0        1            0    0    0    0    0    0    0            0          */
    gpio->GPDDAT = 0x100;
    gpio->GPDCON = 0x55555555;
    gpio->GPDUP  = 0xFFFF;


	/* Port E */
	/* Ports  : GPE15  GPE14  GPE13         GPE12         GPE11          GPE10   GPE9    GPE8    GPE7    GPE6  GPE5  GPE4   GPE3   GPE2  GPE1    GPE0     */
	/* Signal : IICSDA IICSCL MZIO_SPI_CLK  MZIO_SPI_MOSI MZIO_SPI_MISO  SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO I2SSDI CDCLK I2SSCLK I2SLRCK  */
	/* Setting: OUT    OUT    OUT           OUT           IN             SD      SD      SD      SD      SD    SD    OUT    IN     OUT   OUT     OUT      */
	/* Binary : 01     01     01            01            00             10      10      10      10      10    10    01     00     01    01      01       */
	/* PU_OFF : 1      1      1             1             1              1       1       1       1       1     1     1      1      1     1       1        */
	/* DATA   : 1      1      0             0             0              0       0       0       0       0     0     0      0      0     0       0        */
    gpio->GPEDAT = 0xC000;
    gpio->GPECON = 0x552AA915;
    gpio->GPEUP  = 0xFFFF;


	/* Port F */
	/* Ports  : GPF7         		GPF6        			GPF5 							GPF4      			GPF3        		GPF2      			GPF1          GPF0        */
	/* Signal : MZIO_RDY_IREQ_N KP_IREQN_WAKEUP  	Wless_WIFI_IREQn  Wless_BT_IREQn 	KP_SideButtonR  KP_SideButtonL  KP_BLIGHTKEY  KP_POWERKEY */
	/* Setting: IN           		IN          			IN  							IN        			IN          		IN      				IN         	  IN          */
	/* Binary : 00              00               	00                00             	00              00              00            10          */
	/* PU_OFF : 1            		1           			1    							1         			1           		1         			1             1           */
	/* DATA   : 0               0                	0                 0              	0               0               0             0           */
    gpio->GPFDAT = 0x0;     
    gpio->GPFCON = 0x02;
    gpio->GPFUP  = 0xFF;


  /* Port G */
	/* Ports  : GPG15 GPG14 GPG13 GPG12   GPG11          GPG10   GPG9    GPG8   GPG7     GPG6      GPG5      GPG4   GPG3          GPG2      GPG1   GPG0          		 */
	/* Signal : BOOT	BOOT	BOOT	BATT_SW MZ_SPI_DRN_PWM BT_CTSn BT_RTSn SD_CDn WIFI_CLK WIFI_MOSI WIFI_MISO LCD_ON CRADLE_WAKEUP MZ_SPI_SS SD_ENn CRADLE_HOTSYNCINT */
	/* Setting: IN    IN    IN    IN	    IN             IN      OUT     IN     OUT      OUT       IN        OUT    IN	          IN        OUT	   IN             	 */
	/* Binary : 00    00    00    00      00             00      01      00     01       01        00        01     10            00        01     10            		 */
	/* PU_OFF : 1     1     1     1       1              1       1       0      1        1         1         1      1             1         1      1             		 */
	/* DATA   : 0     0     0     0       0              0       0       0      0        0         0         1      0             0         0      0             		 */
	gpio->GPGDAT = 0x1;
	gpio->GPGCON = 0x00045186;
	gpio->GPGUP	 = 0xFEFF;


	/* Port H */
	/* Ports  : GPH10               GPH9        GPH8    GPH7           GPH6             GPH5               GPH4              GPH3             GPH2            GPH1               GPH0   						*/
	/* Signal : USB_CRADLE_HOST_EN  3V3_EXT_ENn SD_WP   MZIO_UART1_RX  MZIO_UART1_TX    Wless_BTUART_RX    Wless_BTUART_TX   C_MZIO_UART2_RX  CMZIO_UART2_TX  C_MZIO_UART2_RTSN  C_MZIO_UART2_CTSN  */
	/* Setting: OUT                 OUT         IN      IN             OUT              IN                 OUT               IN               OUT             OUT                IN   							*/
	/* Binary : 01                  01          00      10             10               00                 01                00               01              01                 00    							*/
  /* PU_OFF : 1                   1           1       1              1                1                  1                 1                1               1                  1     							*/
	/* DATA   : 0                   0           0       0              0                0                  0                 0                0               0                  0     							*/
	gpio->GPHDAT = 0x0;
	gpio->GPHCON = 0x14A114;
	gpio->GPHUP	 = 0x7FF;


	/* Port J */
	/* Ports  : GPJ12   GPJ11       GPJ10	   GPJ9      GPJ8     GPJ7      GPJ6      GPJ5      GPJ4      GPJ3      GPJ2      GPJ1      GPJ0      */
	/* Signal : CAM_RST CAM_CLK_OUT CAM_HREF CAM_VSYNC CAM_PCLK CAM_DATA7 CAM_DATA6 CAM_DATA5 CAM_DATA4 CAM_DATA3 CAM_DATA2 CAM_DATA1 CAM_DATA0 */
	/* Setting: OUT     OUT         OUT      OUT       OUT      OUT       OUT       OUT       OUT       OUT       OUT       OUT       OUT  			*/
	/* Binary : 01      01          01       01        01       01        01        01        01        01        01        01        01   			*/
	/* PU_OFF : 1       1           1        1         1        1         1         1         1         1         1         1         1    			*/
	/* DATA   : 0       0           0        0         0        0         0         0         0         0         0         0         0    			*/
	gpio->GPJDAT = 0;
	gpio->GPJCON = 0x1555555;
	gpio->GPJUP	 = 0x1FFF;


	gpio->EXTINT0=0x22222224;    // EINT[7:0]      // EINT0 rising edge
	gpio->EXTINT1=0x22222222;
	gpio->EXTINT2=0x22222222;


	/* arch number from kernel post 2.6.28 */
#ifndef MACH_TYPE_MINI2440
#define MACH_TYPE_MINI2440 1999
#endif
	gd->bd->bi_arch_number = MACH_TYPE_MINI2440;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x30000100;

	icache_enable();
	dcache_enable();

	return 0;
}



#define MVAL		(0)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control


//TFT 240320
#define LCD_XSIZE_TFT_240320 	(240)	
#define LCD_YSIZE_TFT_240320 	(320)

//TFT240320
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)

//Timing parameter for NEC3.5"
#define VBPD_240320		(3)		
#define VFPD_240320		(10)
#define VSPW_240320		(1)

#define HBPD_240320		(5)
#define HFPD_240320		(2)
#define HSPW_240320		(36)

#define CLKVAL_TFT_240320	(3) 	
//FCLK=101.25MHz,HCLK=50.625MHz,VCLK=6.33MHz


void board_video_init(GraphicDevice *pGD) 
{ 
    S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD(); 
	 
    /* FIXME: select LCM type by env variable */ 
	 
	/* Configuration for GTA01 LCM on QT2410 */ 
	lcd->LCDCON1 = 0x00000378; /* CLKVAL=4, BPPMODE=16bpp, TFT, ENVID=0 */ 
	
//    lcd->LCDCON2 = 0x014fc141; 
//	lcd->LCDCON3 = 0x0098ef13; 
//	lcd->LCDCON4 = 0x00000d05; 
	lcd->LCDCON5 = 0x00000f09; 

	lcd->LCDCON2 = (VBPD_240320<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320<<6)|(VSPW_240320); 
	lcd->LCDCON3 = (HBPD_240320<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320); 
	lcd->LCDCON4 = (MVAL<<8)|(HSPW_240320); 
	
   
    lcd->LPCSEL  = 0x00000000; 
} 

int dram_init (void)
{
	S3C24X0_MEMCTL * const mem = S3C24X0_GetBase_MEMCTL();
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	/* if the early bootloader found 128MB, lets tell the kernel */
	if ((mem->BANKCON[6] & 0x3) == 0x2)
		gd->bd->bi_dram[0].size = 128*1024*1024;
	else
		gd->bd->bi_dram[0].size = 64*1024*1024;

	return 0;
}

/* The sum of all part_size[]s must equal to the NAND size, i.e., 0x4000000 */

unsigned int dynpart_size[] = {
    CFG_UBOOT_SIZE, 0x20000, 0x500000, 0xffffffff, 0 };
char *dynpart_names[] = {
    "u-boot", "u-boot_env", "kernel", "rootfs", NULL };


