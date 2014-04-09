/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
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

/* #define	DEBUG	*/

#include <common.h>
#include <watchdog.h>
#include <command.h>
#ifdef CONFIG_MODEM_SUPPORT
#include <malloc.h>		/* for free() prototype */
#endif

#ifdef CFG_HUSH_PARSER
#include <hush.h>
#endif

#include <post.h>

#if defined(CONFIG_SILENT_CONSOLE) || defined(CONFIG_POST)
DECLARE_GLOBAL_DATA_PTR;
#endif

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
#ifndef CONFIG_MEZ1500
void inline __show_boot_progress (int val) {}
void inline show_boot_progress (int val) __attribute__((weak, alias("__show_boot_progress")));
#else
void inline show_boot_progress (int val) {}
#endif

#if defined(CONFIG_BOOT_RETRY_TIME) && defined(CONFIG_RESET_TO_RETRY)
extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);		/* for do_reset() prototype */
#endif

extern int do_bootd (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);


#define MAX_DELAY_STOP_STR 32

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
static int abortboot(int);
#endif

#undef DEBUG_PARSER

char        console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/
int nobootdelay;

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen);
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char   tab_seq[] = "        ";		/* used to expand TABs	*/
#endif /* CONFIG_CMDLINE_EDITING */

#ifdef CONFIG_BOOT_RETRY_TIME
static uint64_t endtime = 0;  /* must be set, default is instant timeout */
static int      retry_time = -1; /* -1 so can call readline before main_loop */
#endif

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())

#ifndef CONFIG_BOOT_RETRY_MIN
#define CONFIG_BOOT_RETRY_MIN CONFIG_BOOT_RETRY_TIME
#endif

#ifdef CONFIG_MODEM_SUPPORT
int do_mdm_init = 0;
extern void mdm_init(void); /* defined in board.c */
#endif

#ifdef CFG_PREBOOT_OVERRIDE
extern char *preboot_override;
#endif




#include <s3c2440.h>



// 2010-03-26 SV: Added for ROM update.
#define UInt8		uint8_t
#define UInt16	uint16_t
#define UInt32	uint32_t

#include <Garnet_S3C2440_lcd.h>

extern void HALDelay(uint32_t microseconds);

extern ulong g_size_rw_fsload;
extern ulong g_size_rw_nand;

extern void DisplayPicture(UInt16 *bmpPtr, int xPos, int yPos, int xSize, int ySize, int xOffset, int yOffset);
extern const unsigned char bmpLoading[];
extern const unsigned char bmpUpdatingROM[];
extern const unsigned char bmpFailed[];

extern const unsigned char bmpFont[];


int LeftSideBtnPressed, RightSideBtnPressed, BacklightBtnPressed;

extern ulong g_size_rw_fsload;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <version.h>	

#define COMPILE_HOUR (((__TIME__[0]-'0')*10) + (__TIME__[1]-'0'))
#define COMPILE_MINUTE (((__TIME__[3]-'0')*10) + (__TIME__[4]-'0'))
#define COMPILE_SECOND (((__TIME__[6]-'0')*10) + (__TIME__[7]-'0'))




#define YEAR ((((__DATE__ [7]-'0')*10+(__DATE__ [8]-'0'))*10+(__DATE__ [9]-'0'))*10+(__DATE__ [10]-'0'))

/* Month: 0 - 11 */
#define MONTH (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 0 : 5) \
	            : __DATE__ [2] == 'b' ? 1 \
              : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
              : __DATE__ [2] == 'y' ? 4 \
              : __DATE__ [2] == 'l' ? 6 \
              : __DATE__ [2] == 'g' ? 7 \
              : __DATE__ [2] == 'p' ? 8 \
              : __DATE__ [2] == 't' ? 9 \
              : __DATE__ [2] == 'v' ? 10 : 11)

#define DAY ((__DATE__ [4]==' ' ? 0 : __DATE__ [4]-'0')*10+(__DATE__[5]-'0'))


unsigned char GetCompileHour(void)
{
  unsigned char hour=COMPILE_HOUR;
  return(hour);
}

unsigned char GetCompileMinute(void)
{
  unsigned char minute=COMPILE_MINUTE;
  return(minute);
}

unsigned char GetCompileSecond(void)
{
  unsigned char second=COMPILE_SECOND;
  return(second);
}

unsigned short GetCompileYear(void)
{
  unsigned short year=YEAR;
  return(year);
}

unsigned char GetCompileMonth(void)
{
  unsigned short month=MONTH;
  return(month + 1);
}

unsigned char GetCompileDay(void)
{
  unsigned short day=DAY;
  return(day);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------






//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//#include <stdarg.h>

#define BMP_FONT_IMG_WIDTH		128
#define BMP_FONT_IMG_HEIGHT		78

#define BMP_FONT_CELL_WIDTH		8
#define BMP_FONT_CELL_HEIGHT	13

#define BMP_FONT_CELL_SPACING	8
#define BMP_FONT_LINE_SPACING	13

#define BMP_FONT_1ST_CHAR			32	// 0x20 ' '
#define BMP_FONT_LST_CHAR			127	// 0x7F

#define BMP_FONT_TOTAL_CHARS	(BMP_FONT_IMG_WIDTH * BMP_FONT_IMG_HEIGHT) / (BMP_FONT_CELL_WIDTH * BMP_FONT_CELL_HEIGHT)

#define BMP_FONT_NUM_COLS			BMP_FONT_IMG_WIDTH / BMP_FONT_CELL_WIDTH
#define BMP_FONT_NUM_ROWS			BMP_FONT_IMG_HEIGHT / BMP_FONT_CELL_HEIGHT

int lcd_x1, lcd_y1, lcd_x2, lcd_y2;

int lcd_CallBack_EN = 0;

//-----------------------------------------------------------------------------
//
void lcd_Putc(char ch)
{
// A pointer to the font image.	
	UInt16* Font16Ptr;
	
// Somewhere to build the char image.	
	UInt16 bmp_char[BMP_FONT_CELL_WIDTH *  BMP_FONT_CELL_HEIGHT];
	
	int bmp_char_index = 0;
	
	int	x, start, end, row, col;
	
// Don't try to display a char that isn't there.
	if((ch < BMP_FONT_1ST_CHAR) || (ch >  BMP_FONT_LST_CHAR))
		return;

// Calc row & col
	row = (int)((int)(ch - BMP_FONT_1ST_CHAR) / (int)(BMP_FONT_NUM_COLS));
	col = (int)((int)(ch - BMP_FONT_1ST_CHAR) - (int)(row * (int)(BMP_FONT_NUM_COLS)));

// Point to the correct row within the bmp
	Font16Ptr = ((UInt16*) bmpFont) + ((BMP_FONT_IMG_WIDTH * BMP_FONT_CELL_HEIGHT) * row);
	
// Calculate trigger points.
	start = col * BMP_FONT_CELL_WIDTH;	
	end   = start + BMP_FONT_CELL_WIDTH;	
	
// Transfer the data we want
	for(x = 0; x < (int)(BMP_FONT_IMG_WIDTH * BMP_FONT_CELL_HEIGHT); x++)
	{
		if((x >= start) && (x < end))
		{
			bmp_char[bmp_char_index] = *Font16Ptr;
			bmp_char_index++;
		}
		
		Font16Ptr++;
		
		if(x >= end)
		{
			// Re-calculate trigger points.
			start += (int)((int)(BMP_FONT_CELL_WIDTH) * (int)(BMP_FONT_NUM_COLS));
			end = start + BMP_FONT_CELL_WIDTH;
		}
	}
	
	DisplayPicture(bmp_char, LCD_XSIZE/2, LCD_YSIZE/2, BMP_FONT_CELL_WIDTH, BMP_FONT_CELL_HEIGHT, -LCD_XSIZE/2 + lcd_x2, -LCD_YSIZE/2 + lcd_y2);
}
//-----------------------------------------------------------------------------
//
void lcd_GotoXY(int x, int y)
{
	lcd_x1 = x;
	lcd_x2 = x;
	lcd_y1 = y;
	lcd_y2 = y;
}
//-----------------------------------------------------------------------------
//
void lcd_Puts(const char *s)
{
	printf("%s\n", s);
	
// Centre justify
	lcd_x2 = lcd_x1 - ((strlen(s)/2) * BMP_FONT_CELL_SPACING);
	
	while(*s)
	{
		if(*s == '\r')
		{
			//lcd_x2 = lcd_x1;
			s++;
			continue;
		}	

		if(*s == '\n')
		{
			lcd_y2 += (int)(BMP_FONT_LINE_SPACING);
			//lcd_x2 = lcd_x1;
			s++;
			continue;
		}	

		lcd_Putc(*s);
		lcd_x2 += (int)(BMP_FONT_CELL_SPACING);
		s++;
	}
}
//-----------------------------------------------------------------------------
// Called from nand R & W
//
void lcd_CallBack(int flg, int value)
{
	char str[CFG_CBSIZE];
	
	if(!lcd_CallBack_EN) 
		return;
	
	switch(flg)
	{
		case 1:
			sprintf(str, "Loading: %d%%\r", value);
			break;
			
		case 2:
			sprintf(str, " Writing: %d%%\r", value);
			break;
	}
	
	lcd_Puts(str);
}
//-----------------------------------------------------------------------------
//
// Formatting needs some work.
/*
void lcd_printf(char *s, ...)
{
	char str[CFG_CBSIZE];
	
	va_list 	args;
	
	va_start(args, s);
  sprintf(str, s, args);
  va_end(args);

	lcd_Puts(str);
}
*/
//-----------------------------------------------------------------------------
//
void lcd_uboot_timestamp(void)
{
	char outstr[64];
	char uboot_version[64];
	char *p;

// Get rid of the MEZ1500 garbage	
	strcpy(uboot_version, U_BOOT_VERSION);
	p = strstr(uboot_version, "-");
	if(p)p = strstr(p+1, "-");
	if(p)*p = 0;	

	sprintf(outstr, "%s %d%.2d%.2d%.2d%.2d\n", uboot_version, GetCompileYear(), GetCompileMonth(), GetCompileDay(), GetCompileHour(), GetCompileMinute());
	lcd_Puts(outstr);

// Also place a copy in memory for Garnet DeviceInfo app	
	p = (char *)0x33200000;		
	
	strcpy(p, outstr);
}
//-----------------------------------------------------------------------------
//
void ClrSDRAM(unsigned int memaddr, unsigned int size)
{
	unsigned int *p;
	
	p = (unsigned int *)memaddr;
	
	size /= 4;
	
	while(size)
	{
		*p = 0;
		p++;
		size--;
	}
}
//-----------------------------------------------------------------------------
//
void DbugPin(int state)
{
	S3C24X0_GPIO * const gpioP = S3C24X0_GetBase_GPIO();
	
	if(state)
		gpioP->GPGDAT |= (1 << 2);
	else
		gpioP->GPGDAT &= ~(1 << 2);
}
//-----------------------------------------------------------------------------

unsigned int ChargerAdapterConnected(void)
{
	S3C24X0_GPIO * const gpioP = S3C24X0_GetBase_GPIO();
	
	if(gpioP->GPBDAT & (1 << 10))
		return 0;
	else
		return 1;
}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot
 *          1 - got key string, abort
 */

#ifdef CONFIG_MENUKEY
static int menukey = 0;
#endif

int display_banner (void);
void enable_putc(void);

// ----------------------------------------------------------------------------
//
// 2010-03-26 SV: Changed to read a HW button pressed.
// 2010-05-20 SV: Updated.
//
static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;
	
	S3C24X0_GPIO * const gpioP = S3C24X0_GetBase_GPIO();
	
	LeftSideBtnPressed   	= 0;
	RightSideBtnPressed  	= 0;	
	BacklightBtnPressed		= 0;
	
// Setup GPF1, GPF2 and GPF3 for read, (BL, left & right side btns respectively on the MEZ1500).
	gpioP->GPFCON &= ~((0x03 << 6) | (0x03 << 4) | (0x03 << 2)); 
	gpioP->GPFUP  |=   (0x01 << 3) | (0x01 << 2) | (0x01 << 1);
	
// Read btns & debounce (0 = pressed).	
	if((gpioP->GPFDAT & 0x08) || (gpioP->GPFDAT & 0x04) || (gpioP->GPFDAT & 0x02))
	{
		HALDelay(1000000);
		
//		if((gpioP->GPFDAT & 0x08) && !(gpioP->GPFDAT & 0x04))
//		{
//     enable_putc();
//			display_banner();
//			
//			printf("\nRIGHT SIDE BTN PRESSED!\n");
//			
//			bootdelay = 5;
//		}		
		
//		if(gpioP->GPFDAT & 0x02)
//		{
//			printf("\nBACKLIGHT BTN PRESSED!\n");
//			printf("RUN FACTORY ROM...\n");
//			BacklightBtnPressed = 1;
//			return 0;
//		}
		
		if((gpioP->GPFDAT & 0x08) && (gpioP->GPFDAT & 0x04))
		{
			printf("\nLEFT & RIGHT SIDE BTNS PRESSED!\n");
			//printf("UPDATE UBOOT FROM SD CARD...\n");
			LeftSideBtnPressed = 1;
			RightSideBtnPressed = 1;
			return 0;
		}
		
//		if(!(gpioP->GPFDAT & 0x08) && (gpioP->GPFDAT & 0x04))
//		{
//			printf("\nLEFT SIDE BTN PRESSED!\n");
//			printf("UPDATE ROM FROM SD CARD...\n");
//			LeftSideBtnPressed = 1;
//			return 0;
//		}
	}
// ----------------------------------------------------------------------------	

	printf("Hit spacebar to stop autoboot: %2d ", bootdelay);

	while ((bootdelay > 0) && (!abort))
	{
		--bootdelay;

		if (tstc())
		{	/* we got a key press	*/
			if(getc() == 0x20)						// 2010-05-31 SV: Now only respond to this char.
			{
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
				break;
			}
		}

		HALDelay(1000000);

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

	return abort;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if 0
//-----------------------------------------------------------------------------
/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot
 *          1 - got key string, abort
 */
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
# if defined(CONFIG_AUTOBOOT_KEYED)
static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;
	uint64_t etime = endtick(bootdelay);
	struct {
		char* str;
		u_int len;
		int retry;
	}
	delaykey [] = {
		{ str: getenv ("bootdelaykey"),  retry: 1 },
		{ str: getenv ("bootdelaykey2"), retry: 1 },
		{ str: getenv ("bootstopkey"),   retry: 0 },
		{ str: getenv ("bootstopkey2"),  retry: 0 },
	};

	char presskey [MAX_DELAY_STOP_STR];
	u_int presskey_len = 0;
	u_int presskey_max = 0;
	u_int i;

#  ifdef CONFIG_AUTOBOOT_PROMPT
	printf(CONFIG_AUTOBOOT_PROMPT, bootdelay);
#  endif

#  ifdef CONFIG_AUTOBOOT_DELAY_STR
	if (delaykey[0].str == NULL)
		delaykey[0].str = CONFIG_AUTOBOOT_DELAY_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_DELAY_STR2
	if (delaykey[1].str == NULL)
		delaykey[1].str = CONFIG_AUTOBOOT_DELAY_STR2;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR
	if (delaykey[2].str == NULL)
		delaykey[2].str = CONFIG_AUTOBOOT_STOP_STR;
#  endif
#  ifdef CONFIG_AUTOBOOT_STOP_STR2
	if (delaykey[3].str == NULL)
		delaykey[3].str = CONFIG_AUTOBOOT_STOP_STR2;
#  endif

	for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
		delaykey[i].len = delaykey[i].str == NULL ?
				    0 : strlen (delaykey[i].str);
		delaykey[i].len = delaykey[i].len > MAX_DELAY_STOP_STR ?
				    MAX_DELAY_STOP_STR : delaykey[i].len;

		presskey_max = presskey_max > delaykey[i].len ?
				    presskey_max : delaykey[i].len;

#  if DEBUG_BOOTKEYS
		printf("%s key:<%s>\n",
		       delaykey[i].retry ? "delay" : "stop",
		       delaykey[i].str ? delaykey[i].str : "NULL");
#  endif
	}

	/* In order to keep up with incoming data, check timeout only
	 * when catch up.
	 */
	while (!abort && get_ticks() <= etime) {
		for (i = 0; i < sizeof(delaykey) / sizeof(delaykey[0]); i ++) {
			if (delaykey[i].len > 0 &&
			    presskey_len >= delaykey[i].len &&
			    memcmp (presskey + presskey_len - delaykey[i].len,
				    delaykey[i].str,
				    delaykey[i].len) == 0) {
#  if DEBUG_BOOTKEYS
				printf("got %skey\n",
				       delaykey[i].retry ? "delay" : "stop");
#  endif

#  ifdef CONFIG_BOOT_RETRY_TIME
				/* don't retry auto boot */
				if (! delaykey[i].retry)
					retry_time = -1;
#  endif
				abort = 1;
			}
		}

		if (tstc()) {
			if (presskey_len < presskey_max) {
				presskey [presskey_len ++] = getc();
			}
			else {
				for (i = 0; i < presskey_max - 1; i ++)
					presskey [i] = presskey [i + 1];

				presskey [i] = getc();
			}
		}
	}
#  if DEBUG_BOOTKEYS
	if (!abort)
		puts("key timeout\n");
#  endif

#ifdef CONFIG_SILENT_CONSOLE
	if (abort)
		gd->flags &= ~GD_FLG_SILENT;
#endif

	return abort;
}

# else	/* !defined(CONFIG_AUTOBOOT_KEYED) */

#ifdef CONFIG_MENUKEY
static int menukey = 0;
#endif

static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;

#ifdef CONFIG_MENUPROMPT
	printf(CONFIG_MENUPROMPT, bootdelay);
#else
	printf("Hit any key to stop autoboot: %2d ", bootdelay);
#endif

#if defined CONFIG_ZERO_BOOTDELAY_CHECK
	/*
	 * Check if key already pressed
	 * Don't check if bootdelay < 0
	 */
	if (bootdelay >= 0) {
		if (tstc()) {	/* we got a key press	*/
			(void) getc();  /* consume input	*/
			puts ("\b\b\b 0");
			abort = 1;	/* don't auto boot	*/
		}
	}
#endif

	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				break;
			}
			udelay(10000);
		}

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

#ifdef CONFIG_SILENT_CONSOLE
	if (abort)
		gd->flags &= ~GD_FLG_SILENT;
#endif

	return abort;
}
# endif	/* CONFIG_AUTOBOOT_KEYED */
#endif	/* CONFIG_BOOTDELAY >= 0  */
//-----------------------------------------------------------------------------
#endif








//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Ported from Garnet code.

#include <Garnet_sysdefs.h>
#include <Garnet_S3C2440_adc.h>

extern void HALDelay(uint32_t microseconds);


#define BATT_SAFE_LEVEL	601			 // 6.2V (batt) * (100K/320K) * 1024 / (Vdd = 3.3V)

#define bv_chan		ADCCONch0

UInt16 ReadADC(UInt8 chan)
{
	volatile HwrS3C2440_ADC_RegPtr adcP = (HwrS3C2440_ADC_RegPtr)S3C2440_ADC_BASE;

	UInt16 data;

//	gpioP->GPJDAT |= bGPJDAT_ADC_ENABLE;	// enable battery monitor circuit

	adcP->ADCDLY = 250;	
	adcP->ADCCON = (bADCCONstdby | (ADCCON_PRSCVL_bv << 6) | (chan << 3));	
	adcP->ADCCON |= bADCCONprscen;			// then enable the prescaler
	HALDelay(3);												// wait min. 3 ADC clocks
	adcP->ADCCON &= ~bADCCONstdby;			// before switching from stdby to normal operation

	// Start the ADC conversion (after programmed delay)
	adcP->ADCCON |= bADCCONenStart;

	// Wait for end of conversion
	while (!(adcP->ADCCON & bADCCONecflg));

	data = (adcP->ADCDAT0 & 0x3FF);				// raw, 10-bit
	
	// Put ADC into standby mode
	adcP->ADCCON |= bADCCONstdby;

//	gpioP->GPJDAT &= ~bGPJDAT_ADC_ENABLE;		// disable battery monitor circuit

	return data;	
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




// ----------------------------------------------------------------------------
//
//	Verify the checksum of entire ROM
// The sum of all words, including the csum word, should be 0
unsigned int DoROMcsum (void)
{
#define		RAM_START_ADDR			0x33240000
#define  	UBOOT_SIZE_ADDR    	0x33240054		// length of ROM in bytes
	
	
	UInt32	*pROM;
	UInt32	size, csum, i;
	
	pROM = (UInt32 *)UBOOT_SIZE_ADDR;
	
	size = (*pROM) >> 2;			// convert from bytes to words

	pROM = (UInt32	*)RAM_START_ADDR;				// point to start of ROM
	csum = 0;
	for (i=0; i<size; i++)
			csum += *pROM++;

	if(csum == 0)
		printf("Checksum Ok!\n"	);
	else
		printf("Checksum bad!\n");

	return (csum == 0);
}





/****************************************************************************/

void main_loop (void)
{
	
#ifndef CFG_HUSH_PARSER
	static char lastcommand[CFG_CBSIZE] = { 0, };
	int len;
	int rc = 1;
	int flag;
#endif

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	char *s;
	int bootdelay;
#endif
#if defined(CONFIG_PREBOOT) || defined(CFG_PREBOOT_OVERRIDE)
	char *p = NULL;
#endif
#ifdef CONFIG_BOOTCOUNT_LIMIT
	unsigned long bootcount = 0;
	unsigned long bootlimit = 0;
	char *bcs;
	char bcs_set[16];
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#if defined(CONFIG_VFD) && defined(VFD_TEST_LOGO)
	ulong bmp = 0;		/* default bitmap */
	extern int trab_vfd (ulong bitmap);

#ifdef CONFIG_MODEM_SUPPORT
	if (do_mdm_init)
		bmp = 1;	/* alternate bitmap */
#endif
	trab_vfd (bmp);
#endif	/* CONFIG_VFD && VFD_TEST_LOGO */

#ifdef CONFIG_BOOTCOUNT_LIMIT
	bootcount = bootcount_load();
	bootcount++;
	bootcount_store (bootcount);
	sprintf (bcs_set, "%lu", bootcount);
	setenv ("bootcount", bcs_set);
	bcs = getenv ("bootlimit");
	bootlimit = bcs ? simple_strtoul (bcs, NULL, 10) : 0;
#endif /* CONFIG_BOOTCOUNT_LIMIT */

#ifdef CONFIG_MODEM_SUPPORT
	debug ("DEBUG: main_loop:   do_mdm_init=%d\n", do_mdm_init);
	if (do_mdm_init) {
		char *str = strdup(getenv("mdm_cmd"));
		setenv ("preboot", str);  /* set or delete definition */
		if (str != NULL)
			free (str);
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */

#ifdef CONFIG_VERSION_VARIABLE
	{
		extern char version_string[];

		setenv ("ver", version_string);  /* set version variable */
	}
#endif /* CONFIG_VERSION_VARIABLE */

#ifdef CFG_HUSH_PARSER
	u_boot_hush_start ();
#endif

#ifdef CONFIG_AUTO_COMPLETE
	install_auto_complete();
#endif

#if defined(CONFIG_PREBOOT) || defined(CFG_PREBOOT_OVERRIDE)
#ifdef CONFIG_PREBOOT
	p = getenv ("preboot");
#endif
#ifdef CFG_PREBOOT_OVERRIDE
	if (preboot_override) {
		/* for convenience, preboot_override may end in \n, not \0 */
		p = strchr(preboot_override, '\n');
		if (p)
			*p = 0;
		/* make sure we can overwrite the load area if we want to */
		p = strdup(preboot_override);
		/* clean the image in case we want to flash it */
		preboot_override = NULL;
	}
#endif /* CFG_PREBOOT_OVERRIDE */
	if (p) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif
		
# ifndef CFG_HUSH_PARSER
		run_command (p, 0);
# else
		parse_string_outer(p, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT || CFG_PREBOOT_OVERRIDE */

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	s = getenv ("bootdelay");
	bootdelay = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_BOOTDELAY;

	debug ("### main_loop entered: bootdelay=%d\n\n", bootdelay);

# ifdef CONFIG_BOOT_RETRY_TIME
	init_cmd_timeout ();
# endif	/* CONFIG_BOOT_RETRY_TIME */

#ifdef CONFIG_POST
	if (gd->flags & GD_FLG_POSTFAIL) {
		s = getenv("failbootcmd");
	}
	else
#endif /* CONFIG_POST */
#ifdef CONFIG_BOOTCOUNT_LIMIT
	if (bootlimit && (bootcount > bootlimit)) {
		printf ("Warning: Bootlimit (%u) exceeded. Using altbootcmd.\n",
		        (unsigned)bootlimit);
		s = getenv ("altbootcmd");
	}
	else
#endif /* CONFIG_BOOTCOUNT_LIMIT */
		s = getenv ("bootcmd");

	debug ("### main_loop: bootcmd=\"%s\"\n", s ? s : "<UNDEFINED>");

	if (!nobootdelay && bootdelay >= 0 && s && !abortboot (bootdelay)) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif










//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

		//run_command (s, 0);

// 2010-03-26 SV: Added for ROM update.

// Some debug.
	printf("\nRaw batt level = %d\n", ReadADC(bv_chan));
	
// Start displaying here...	
	lcd_GotoXY(LCD_XSIZE/2, 150);
	
	lcd_uboot_timestamp();

/*
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Update uboot?

	if(LeftSideBtnPressed && RightSideBtnPressed)
	{
			printf("UPDATE UBOOT FROM SD CARD...\n");
			run_command ("mmcinit", 0);
			run_command ("fatload mmc 0:1 0x33240000 mez1500_rev2d_uboot.bin", 0);
			run_command ("nand erase u-boot", 0);
			run_command ("nand write 0x33240000 0", 0);
			reset_cpu(0);
	}
	

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
*/


//.............................................................................
//
// Update uboot?

	if(LeftSideBtnPressed && RightSideBtnPressed)
	{
		if(ReadADC(bv_chan) >= BATT_SAFE_LEVEL || ChargerAdapterConnected())
		{	
			lcd_Puts("Updating all if found\n");
			
			run_command ("mmcinit", 0);
			
			lcd_Puts("Read uboot bin:\n");
  		lcd_Puts("mez1500_rev2d_uboot.bin\n");					
  		
  		ClrSDRAM(0x33240000, 0x400000);
			
			g_size_rw_fsload = 0;
			run_command ("fatload mmc 0:1 0x33240000 mez1500_rev2d_uboot.bin", 0);
			
			if(g_size_rw_fsload && DoROMcsum())
			{
				char str[128];
				lcd_Puts("Write uboot bin to nand.\n");					
				run_command ("nand erase u-boot", 0);
				sprintf(str,"nand write.e 0x33240000 0 0x%x", g_size_rw_fsload);
				run_command (str, 0);
				//run_command ("nand write 0x33240000 0", 0);
				
				run_command ("reset", 0);
			}
			else
			{
				lcd_Puts("No bin found on sdcard!\n");
			}
		}
		else
		{
			lcd_Puts("Update FAILED!\n");
			lcd_Puts("Battery voltage too low!\n");
		}
	}
	
//.............................................................................

	lcd_Puts("\n");
	lcd_Puts("* Booting kernel *\n");


# ifndef CFG_HUSH_PARSER
		run_command (s, 0);
# else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}

# ifdef CONFIG_MENUKEY
	if (menukey == CONFIG_MENUKEY) {
	    s = getenv("menucmd");
	    if (s) {
# ifndef CFG_HUSH_PARSER
		run_command (s, 0);
# else
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
# endif
	    }
	}
#endif /* CONFIG_MENUKEY */
#endif	/* CONFIG_BOOTDELAY */

#ifdef CONFIG_AMIGAONEG3SE
	{
	    extern void video_banner(void);
	    video_banner();
	}
#endif

	/*
	 * Main Loop for Monitor Command Processing
	 */
#ifdef CFG_HUSH_PARSER
	parse_file_outer();
	/* This point is never reached */
	for (;;);
#else
	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		if (rc >= 0) {
			/* Saw enough of a valid command to
			 * restart the timeout.
			 */
			reset_cmd_timeout();
		}
#endif
		len = readline (CFG_PROMPT);

		flag = 0;	/* assume no special flags for now */
		if (len > 0)
			strcpy (lastcommand, console_buffer);
		else if (len == 0)
			flag |= CMD_FLAG_REPEAT;
#ifdef CONFIG_BOOT_RETRY_TIME
		else if (len == -2) {
			/* -2 means timed out, retry autoboot
			 */
			puts ("\nTimed out waiting for command\n");
# ifdef CONFIG_RESET_TO_RETRY
			/* Reinit board to run initialization code again */
			do_reset (NULL, 0, 0, NULL);
# else
			return;		/* retry autoboot */
# endif
		}
#endif

		if (len == -1)
			puts ("<INTERRUPT>\n");
		else
			rc = run_command (lastcommand, flag);

		if (rc <= 0) {
			/* invalid command or not repeatable, forget it */
			lastcommand[0] = 0;
		}
	}
#endif /*CFG_HUSH_PARSER*/
}

#ifdef CONFIG_BOOT_RETRY_TIME
/***************************************************************************
 * initialize command line timeout
 */
void init_cmd_timeout(void)
{
	char *s = getenv ("bootretry");

	if (s != NULL)
		retry_time = (int)simple_strtol(s, NULL, 10);
	else
		retry_time =  CONFIG_BOOT_RETRY_TIME;

	if (retry_time >= 0 && retry_time < CONFIG_BOOT_RETRY_MIN)
		retry_time = CONFIG_BOOT_RETRY_MIN;
}

/***************************************************************************
 * reset command line timeout to retry_time seconds
 */
void reset_cmd_timeout(void)
{
	endtime = endtick(retry_time);
}
#endif

#ifdef CONFIG_CMDLINE_EDITING

/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(str,n)	do {			\
		printf ("%.*s", n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		256

#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE];

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

#ifndef CONFIG_CMDLINE_EDITING
static void cread_print_hist_list(void)
{
	int i;
	unsigned long n;

	n = hist_num - hist_max;

	i = hist_add_idx + 1;
	while (1) {
		if (i > hist_max)
			i = 0;
		if (i == hist_add_idx)
			break;
		printf("%s\n", hist_list[i]);
		n++;
		i++;
	}
}
#endif /* CONFIG_CMDLINE_EDITING */

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(const char *const prompt, char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long rlen;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	int rc = 0;
	char esc_save[8];

	while (1) {
		rlen = 1;
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
		}
#endif

		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
		case CTL_CH('u'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
#ifdef CONFIG_AUTO_COMPLETE
		case '\t': {
			int num2, col;

			/* do not autocomplete when in the middle */
			if (num < eol_num) {
				getcmd_cbeep();
				break;
			}

			buf[num] = '\0';
			col = strlen(prompt) + eol_num;
			num2 = num;
			if (cmd_auto_complete(prompt, buf, &num2, &col)) {
				col = num2 - num;
				num += col;
				eol_num += col;
			}
			break;
		}
#endif
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}

#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline (const char *const prompt)
{
	return readline_into_buffer(prompt, console_buffer);
}


int readline_into_buffer (const char *const prompt, char * buffer)
{
	char *p = buffer;
#ifdef CONFIG_CMDLINE_EDITING
	unsigned int len=MAX_CMDBUF_SIZE;
	int rc;
	static int initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	puts (prompt);

	rc = cread_line(prompt, p, &len);
	return rc < 0 ? rc : len;
#else
	char * p_buf = p;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = strlen (prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
		}
#endif
		WATCHDOG_RESET();		/* Trigger watchdog, if needed */

#ifdef CONFIG_SHOW_ACTIVITY
		while (!tstc()) {
			extern void show_activity(int arg);
			show_activity(0);
		}
#endif
		c = getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			puts ("\r\n");
			return (p - p_buf);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			p_buf[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				puts (erase_seq);
				--col;
			}
			p = p_buf;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(p_buf, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(p_buf, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CFG_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
#ifdef CONFIG_AUTO_COMPLETE
					/* if auto completion triggered just continue */
					*p = '\0';
					if (cmd_auto_complete(prompt, console_buffer, &n, &col)) {
						p = p_buf + n;	/* reset */
						continue;
					}
#endif
					puts (tab_seq+(col&07));
					col += 8 - (col&07);
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
#endif /* CONFIG_CMDLINE_EDITING */
}

/****************************************************************************/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			puts (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				puts (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				putc (*s);
			}
		}
	} else {
		puts (erase_seq);
		(*colp)--;
	}
	(*np)--;
	return (p);
}
#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#ifdef DEBUG_PARSER
	printf ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#ifdef DEBUG_PARSER
	printf ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/****************************************************************************/

static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CFG_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#ifdef DEBUG_PARSER
	char *output_start = output;

	printf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),
		input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\')) {
				state++;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{') {
				state++;
				varname_start = input;
			} else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				int i;
				char envname[CFG_CBSIZE], *envval;
				int envcnt = input - varname_start - 1;	/* Varname # of chars */

				/* Get the varname */
				for (i = 0; i < envcnt; i++) {
					envname[i] = varname_start[i];
				}
				envname[i] = 0;

				/* Get its value */
				envval = getenv (envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\')) {
				state = 0;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;
	else
		*(output - 1) = 0;

#ifdef DEBUG_PARSER
	printf ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n",
		strlen (output_start), output_start);
#endif
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CFG_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CFG_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	char *argv[CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

#ifdef DEBUG_PARSER
	printf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	puts (cmd ? cmd : "NULL");	/* use puts - string may be loooong */
	puts ("\"\n");
#endif

	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CFG_CBSIZE) {
		puts ("## Command too long!\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

#ifdef DEBUG_PARSER
	printf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
		printf ("token: \"%s\"\n", token);
#endif

		/* find macros in this token and replace them */
		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (finaltoken, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL) {
			printf ("Unknown command '%s' - try 'help'\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			printf ("Usage:\n%s\n", cmdtp->usage);
			rc = -1;
			continue;
		}

#if defined(CONFIG_CMD_BOOTD)
		/* avoid "bootd" recursion */
		if (cmdtp->cmd == do_bootd) {
#ifdef DEBUG_PARSER
			printf ("[%s]\n", finaltoken);
#endif
			if (flag & CMD_FLAG_BOOTD) {
				puts ("'bootd' recursion detected\n");
				rc = -1;
				continue;
			} else {
				flag |= CMD_FLAG_BOOTD;
			}
		}
#endif

		/* OK - call function to do the command */
		if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0) {
			rc = -1;
		}

		repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
			return -1;	/* if stopped then not repeatable */
	}

	return rc ? rc : repeatable;
}

/****************************************************************************/

#if defined(CONFIG_CMD_RUN)
int do_run (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	for (i=1; i<argc; ++i) {
		char *arg;

		if ((arg = getenv (argv[i])) == NULL) {
			printf ("## Error: \"%s\" not defined\n", argv[i]);
			return 1;
		}
#ifndef CFG_HUSH_PARSER
		if (run_command (arg, flag) == -1)
			return 1;
#else
		if (parse_string_outer(arg,
		    FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP) != 0)
			return 1;
#endif
	}
	return 0;
}
#endif
