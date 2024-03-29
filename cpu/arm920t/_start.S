/*
 *  armboot - Startup Code for ARM920 CPU-core
 *
 *  Copyright (c) 2001	Marius Gr�ger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Z�pke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <gj@denx.de>
 *
 * S3C2410 NAND portions
 *  Copyright (c) 2001  MIZI Research, Inc.
 *  Copyright (c) 2006  OpenMoko, Inc. (Harald Welte <laforge@openmmoko.org>
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


#include <config.h>
#include <version.h>
#if defined(CONFIG_S3C2410)
#include <s3c2410.h>
#elif defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442)
#include <s3c2440.h>
#elif defined(CONFIG_S3C2443)
#include <s3c2443.h>
#endif
#include <status_led.h>


#define CONFIG_SYS_CLK_FREQ 12000000

/*
 *************************************************************************
 *
 * Jump vector table as in table 3.1 in [1]
 *
 *************************************************************************
 */


.globl _start
_start:	b       start_code
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq

	.balignl 16,0xdeadbeef


/*
 *************************************************************************
 *
 * Startup Code (called from the ARM reset exception vector)
 *
 * do important init only if we don't start from memory!
 * relocate armboot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */


/* Must follow the .balign above, so we get a well-known address ! */
#ifdef CFG_PREBOOT_OVERRIDE
.globl	preboot_override
preboot_override:
	.word	0
#endif

/* Must follow preboot_override , so we get a well-known address ! */
#ifdef CFG_ENV_OVERRIDE
.globl	env_override
env_override:
	.word	0
#endif

#ifdef CONFIG_S3C2410_NAND_BOOT
.globl	booted_from_nand
booted_from_nand:
	.word	0
_booted_from_nand:
	.word	booted_from_nand
#endif /* CONFIG_S3C2410_NAND_BOOT */

#ifndef CFG_NO_FLASH
.globl booted_from_nor
booted_from_nor:
	.word	0
_booted_from_nor:
	.word	booted_from_nor
_end_if_0:
	.word	__bss_start-_start
#endif /* !CFG_NO_FLASH */

_TEXT_BASE:
	.word	TEXT_BASE

.globl _armboot_start
_armboot_start:
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de
#endif


/*
 * the actual start code
 */

start_code:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	/* in case we run from the s3c24xx NAND stepping stone, the symbols
	 * for LED support are in lib_arm/board.o, i.e. outside of the
	 * steppingstone */
#ifndef CONFIG_S3C2410_NAND_BOOT
	bl coloured_LED_init
	bl red_LED_on
#endif

#if	defined(CONFIG_AT91RM9200DK) || defined(CONFIG_AT91RM9200EK) || defined(CONFIG_AT91RM9200DF)
	/*
	 * relocate exception table
	 */
	ldr	r0, =_start
	ldr	r1, =0x0
	mov	r2, #16
copyex:
	subs	r2, r2, #1
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	bne	copyex
#endif

#if defined(CONFIG_S3C2400) || defined(CONFIG_S3C2410) || defined(CONFIG_S3C2440) || \
    defined(CONFIG_S3C2442) || defined(CONFIG_S3C2443)
	/* turn off the watchdog */

# if defined(CONFIG_S3C2400)
#  define pWTCON		0x15300000
#  define INTMSK		0x14400008	/* Interupt-Controller base addresses */
#  define CLKDIVN	0x14800014	/* clock divisor register */
#elif defined(CONFIG_S3C2410) || defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442)
#  define pWTCON		0x53000000
#  define INTMSK		0x4A000008	/* Interupt-Controller base addresses */
#  define INTSUBMSK	0x4A00001C
#  define CLKDIVN	0x4C000014	/* clock divisor register */
# endif

#if defined(CONFIG_S3C2410)
# define INTSUBMSK_val	0x7ff
# define MPLLCON_val	((0x90 << 12) + (0x7 << 4) + 0x0)	/* 202 MHz */
# define UPLLCON_val	((0x78 << 12) + (0x2 << 4) + 0x3)
# define CLKDIVN_val	3 /* FCLK:HCLK:PCLK = 1:2:4 */
#elif defined(CONFIG_S3C2440)
# define INTSUBMSK_val	0xffff
#if (CONFIG_SYS_CLK_FREQ == 16934400)
# define MPLLCON_val	((0x61 << 12) + (0x1 << 4) + 0x2)	/* 296.35 MHz */
# define UPLLCON_val	((0x3c << 12) + (0x4 << 4) + 0x2)	/*  47.98 MHz */
#elif (CONFIG_SYS_CLK_FREQ == 12000000)
# define MPLLCON_val	((0x44 << 12) + (0x1 << 4) + 0x1)	/* 304.00 MHz */
# define UPLLCON_val	((0x38 << 12) + (0x2 << 4) + 0x2)	/*  48.00 MHz */
#endif
# define CLKDIVN_val	7 /* FCLK:HCLK:PCLK = 1:3:6 */
# define CAMDIVN	0x4C000018
#elif defined(CONFIG_S3C2442)
# define INTSUBMSK_val        0xffff
# if (CONFIG_SYS_CLK_FREQ == 12000000)
#  define MPLLCON_val ((142 << 12) + (7 << 4) + 1)
#  define UPLLCON_val   (( 88 << 12) + (4 << 4) + 2)
# elif (CONFIG_SYS_CLK_FREQ == 16934400)
#  define MPLLCON_val   ((181 << 12) + (14<< 4) + 1)
#  define UPLLCON_val   (( 26 << 12) + (4 << 4) + 1)
# endif
# define CLKDIVN_val  7 /* FCLK:HCLK:PCLK = 1:3:6 */
# define CAMDIVN      0x4C000018
#elif defined(CONFIG_S3C2443)
# define INTSUBMSK_val        0x1fffffff
# define EPLLCON_val  ((40 << 16) | (1 << 8) | (1))           /* 96 MHz */
# define MPLLCON_val  ((81 << 16) | (2 << 8) | (0))           /* 1068 MHz */
# define CLKDIV0_val  ((8 << 9) | (1 << 4) | (1 << 3) | (1 << 2)
#endif

	ldr     r0, =pWTCON
	mov     r1, #0x0
	str     r1, [r0]

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]
# if defined(CONFIG_S3C2410) || defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442) || \
     defined(CONFIG_S3C2443)
	ldr	r1, =INTSUBMSK_val
	ldr	r0, =INTSUBMSK
	str	r1, [r0]
# endif

#if defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442)
	/* Make sure we get FCLK:HCLK:PCLK */
	ldr	r0, =CAMDIVN
	mov	r1, #0
	str	r1, [r0]
#endif

	/* Clock asynchronous mode */
	mrc	p15, 0, r1, c1, c0, 0
	orr	r1, r1, #0xc0000000
	mcr	p15, 0, r1, c1, c0, 0


#if defined(CONFIG_S3C2443)
#define LOCKCON0	0x4c000000
#define LOCKCON1	0x4c000004
#define MPLLCON		0x4c000010
#define EPLLCON		0x4c000018

	ldr	r0, =CLKDIV0
	ldr	r1, =CLKDIV0_val
	str	r1, [r0]

	/* set safe (way too long) locktime for both PLLs */
	ldr	r0, =LOCKCON0
	mov	r1, #0xffffff
	str	r1, [r0]
	ldr	r0, =LOCKCON1
	str	r1, [r0]

	/* configure MPLL */
	ldr	r0, =MPLLCON
	ldr	r1, =MPLLCON_val
	str	r1, [r0]

	/* select MPLL clock out for SYSCLK */
	ldr	r0, =CLKSRC
	ldr	r1, [r0]
	orr	r1, r1, #0x10
	str	r1, [r0]

#if 0
	/* configure EPLL */
	ldr	r0, =EPLLCON
	ldr	r1, =EPLLCON_val
	str	r1, [r0]
#endif

#else /* i.e. 2440, 2410 and 2440 */

#define LOCKTIME	0x4c000000
#define UPLLCON		0x4c000008

	ldr	r0, =LOCKTIME
	mov	r1, #0xffffff
	str	r1, [r0]

	ldr	r0, =UPLLCON
	ldr	r1, =UPLLCON_val
	str	r1, [r0]

	/* Page 7-19, seven nops between UPLL and MPLL */
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	ldr	r1, =MPLLCON_val
	str	r1, [r0, #-4]		/* MPLLCON */

	/* FCLK:HCLK:PCLK */
	ldr	r0, =CLKDIVN
	mov	r1, #CLKDIVN_val
	str	r1, [r0]
#endif

#if 1
	/* enable uart */
	ldr	r0, =0x4c00000c		/* clkcon */
	ldr	r1, =0x7fff0		/* all clocks on */
	str	r1, [r0]

	/* gpio UART2 init */
	ldr	r0, =0x56000070
	ldr	r1, =0x14A114
	str	r1, [r0]

	/* init uart */
	ldr	r0, =0x50008000
	mov	r1, #0x03
	str	r1, [r0]
	ldr	r1, =0x245
	str	r1, [r0, #0x04]
	mov	r1, #0x01
	str	r1, [r0, #0x08]
	mov	r1, #0x00
	str	r1, [r0, #0x0c]
	mov	r1, #0x1a
	str	r1, [r0, #0x28]

	mov		r1, #0xff
uartdly:
	subs	r1, r1, #1
	bpl		uartdly

	mov		r0, #'1'
	bl		tx_char_r0
#endif

#endif	/* CONFIG_S3C2400 || CONFIG_S3C2410 || CONFIG_S3C2440 || CONFIG_S3C2442
	   CONFIG_S3C2443 */

	bl	cpu_init_crit

	mov		r0, #'2'
	bl		tx_char_r0


#if defined(CONFIG_AT91RM9200) || defined(CONFIG_S3C2410) || \
    defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442) || \
    defined(CONFIG_S3C2443)

#ifndef CONFIG_SKIP_RELOCATE_UBOOT
	adr	r0, _start		/* r0 <- current position of code   */

#ifdef CONFIG_S3C2410_NAND_BOOT
					/* are we running from NAND ?	    */
#define	BWSCON	0x48000000
	ldr	r1, =BWSCON		/* Z = CPU booted from NAND	    */
	ldr	r1, [r1]
	tst	r1, #6			/* BWSCON[2:1] = OM[1:0]	    */
	teqeq	r0, #0			/* Z &= running at address 0	    */
	beq	nand_load
#endif /* CONFIG_S3C2410_NAND_BOOT */

relocate:				/* relocate U-Boot to RAM	    */
	teq	r0, #0			/* running at address 0 ?	    */
	bleq	may_resume		/* yes -> do low-level setup	    */

	adr	r0, _start		/* the above may have clobbered r0  */

	ldr	r1, _TEXT_BASE		/* test if we run from flash or RAM */
	cmp     r0, r1                  /* don't reloc during debug         */
	beq     done_relocate

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2		/* r2 <- size of armboot            */
	add	r2, r0, r2		/* r2 <- source end address         */

copy_loop:
	ldmia	r0!, {r3-r10}		/* copy from source address [r0]    */
	stmia	r1!, {r3-r10}		/* copy to   target address [r1]    */
	cmp	r0, r2			/* until source end address [r2]    */
	ble	copy_loop

#ifndef CFG_NO_FLASH
	ldr	r0, _end_if_0		/* are we booting from NOR ? */
	cmp	r0, r2
	ldreq	r0, _booted_from_nor	/* remember that we've booted from  */
	moveq	r1, #1			/* NOR                              */
	streqb	r1, [r0]
#endif /* !CFG_NO_FLASH */

	mov	r0, #0			/* flush v3/v4 cache */
	mcr	p15, 0, r0, c7, c7, 0
	ldr	pc, _done_relocate	/* jump to relocated code */
_done_relocate:
	.word	done_relocate

#ifdef CONFIG_S3C2410_NAND_BOOT
nand_load:
	bl	may_resume		/* low-level setup and resume */

	@ reset NAND
#if defined(CONFIG_S3C2410)
	mov	r1, #S3C2410_NAND_BASE
	ldr	r2, =0xf842		@ initial value enable tacls=3,rph0=6,rph1=0
	str	r2, [r1, #oNFCONF]
	ldr	r2, [r1, #oNFCONF]
	bic	r2, r2, #0x800		@ enable chip
	str	r2, [r1, #oNFCONF]
	mov	r2, #0xff		@ RESET command
	strb	r2, [r1, #oNFCMD]
	mov	r3, #0			@ wait
1:	add	r3, r3, #0x1
	cmp	r3, #0xa
	blt	1b
2:	ldr	r2, [r1, #oNFSTAT]	@ wait ready
	tst	r2, #0x1
	beq	2b
	ldr	r2, [r1, #oNFCONF]
	orr	r2, r2, #0x800		@ disable chip
	str	r2, [r1, #oNFCONF]
#elif defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442)
	mov	r1, #S3C2440_NAND_BASE
	ldr	r2, =0xfff0		@ initial value tacls=3,rph0=7,rph1=7
	ldr	r3, [r1, #oNFCONF]
	orr	r3, r3, r2
	str	r3, [r1, #oNFCONF]

	ldr	r3, [r1, #oNFCONT]
	orr	r3, r3, #1		@ enable nand controller
	str	r3, [r1, #oNFCONT]
#endif /* CONFIG_S3C2440 || CONFIG_S3C2442 */

	ldr	r0, _TEXT_BASE		/* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CFG_MALLOC_LEN	/* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo                        */
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		/* leave 3 words for abort-stack    */

	@ copy u-boot to RAM
	ldr	r0, _TEXT_BASE
	mov     r1, #0x0
	mov	r2, #CFG_UBOOT_SIZE
	bl	nand_read_ll

	tst	r0, #0x0
	beq	ok_nand_read
#ifdef CONFIG_DEBUG_LL
bad_nand_read:
	ldr	r0, STR_FAIL
	ldr	r1, SerBase
	bl	PrintWord
1:	b	1b		@ infinite loop
#endif

ok_nand_read:
#ifdef CONFIG_DEBUG_LL
	ldr	r0, STR_OK
	ldr	r1, SerBase
	bl	PrintWord
#endif

	@ verify
	mov	r0, #0
	@ldr	r1, =0x33f00000
	ldr	r1, _TEXT_BASE
	mov	r2, #0x400	@ 4 bytes * 1024 = 4K-bytes
go_next:
	ldr	r3, [r0], #4
	ldr	r4, [r1], #4
	teq	r3, r4
	bne	notmatch
	subs	r2, r2, #4
	beq	done_nand_read
	bne	go_next
notmatch:
#ifdef CONFIG_DEBUG_LL
	sub	r0, r0, #4
	ldr	r1, SerBase
	bl	PrintHexWord
	ldr	r0, STR_FAIL
	ldr	r1, SerBase
	bl	PrintWord
#endif
1:	b	1b
done_nand_read:
	ldr	r0, _booted_from_nand
	mov	r1, #1
	strb	r1, [r0]
#endif /* CONFIG_S3C2410_NAND_BOOT */
done_relocate:

#if defined(CONFIG_USE_IRQ) && (defined(CONFIG_S3C2410) || \
    defined(CONFIG_S3C2440) || defined(CONFIG_S3C2442))
	/* In the case of the S3C2410, if we've somehow magically (JTAG, ...)
	   ended up in RAM, then that ram is mapped to 0x30000000 and not 0.
	   So we need to copy the interrupt vectors, etc.  */

	mov	r0, #0
	ldr	r1, _TEXT_BASE
	mov	r2, #0x40
irqvec_cpy_next:
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	subs	r2, r2, #4
	bne	irqvec_cpy_next
#endif /* CONFIG_USE_IRQ */

#endif /* CONFIG_SKIP_RELOCATE_UBOOT */
#endif
	/* Set up the stack						    */
stack_setup:
	ldr	r0, _TEXT_BASE		/* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CFG_MALLOC_LEN	/* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo                        */
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		/* leave 3 words for abort-stack    */

clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov 	r2, #0x00000000		/* clear                            */

clbss_l:str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l

	ldr	pc, _start_armboot

_start_armboot:	.word start_armboot


/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */

cpu_init_crit:
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * before relocating, we have to setup RAM timing
	 * because memory timing is board-dependend, you will
	 * find a lowlevel_init.S in your board directory.
	 */
	mov	ip, lr

	bl	lowlevel_init

	mov	lr, ip
	mov	pc, lr

/*
 *************************************************************************
 *
 * may_resume
 *
 * Bring up memory and check if we're coming out of suspend.
 *
 *************************************************************************
 */


may_resume:

	mov	r10, lr			/* we may call cpu_init_crit */

	/* take sdram out of power down */
	ldr	r0, =0x56000080		/* misccr */
	ldr	r1, [ r0 ]
	bic	r1, r1, #(S3C2410_MISCCR_nEN_SCLK0 | S3C2410_MISCCR_nEN_SCLK1 | S3C2410_MISCCR_nEN_SCLKE)
	str	r1, [ r0 ]

	/* ensure signals stabalise */
	mov	r1, #128
1:	subs	r1, r1, #1
	bpl	1b


	bl	cpu_init_crit


	/* ensure some refresh has happened */
	ldr	r1, =0xfffff
1:	subs	r1, r1, #1
	bpl	1b



	mov		r0, #'3'
	bl		tx_char_r0



	/* capture full EINT situation into gstatus 4 */

	ldr	r0, =0x4A000000 /* SRCPND */
	ldr	r1, [ r0 ]
	and	r1, r1, #0xf

	ldr	r0, =0x560000BC /* gstatus4 */
	str	r1, [ r0 ]

	ldr	r0, =0x560000A8 /* EINTPEND */
	ldr	r1, [ r0 ]
	ldr	r0, =0xfff0
	and	r1, r1, r0
	ldr	r0, =0x560000BC /* gstatus4 */
	ldr     r0, [ r0 ]
	orr	r1, r1, r0
	ldr	r0, =0x560000BC /* gstatus4 */
	str	r1, [ r0 ]




	/* test for resume */
	mov		r1, #S3C24X0_GPIO_BASE
	ldr		r0, [r1, #oGSTATUS2]
	tst		r0, #0x02					/* is this resume from power down?	*/
	
	/*  Reset the wakeup status bit so we don't come back here */
	mov			r0, #0x02								@ write '1' to reset SLEEPRST bit
	str			r0, [r1, #oGSTATUS2]


/* If GSTATUS2 wakeup reset bit is set then jump to address */
/* stored in GSTATUS3 by linux prior to sleep 							*/

	/* 'return' to linux address in GSTATUS3	*/
	ldrne		pc, [r1, #oGSTATUS3]

/* not wakeup reset so 'return' to address in R10		*/
	mov	pc, r10




@ *************************************************************************

@ Wait for and read 1 char from UART Rx buffer
@ returns	r0 = received char, r1 = error
@ Modified:	r0, r12

get_char:
	mov		r12,#S3C24X0_UART_BASE
	add 	r12,r12,#0x8000

wt4rx:
	ldr		r0,[r12,#oUTRSTAT]	@ UTRSTATn Tx/Rx status
	tst		r0,#1								@ bit 0 = Rx data ready
	beq		wt4rx								@ wait for received char

	ldrb	r0,[r12,#oURXH]			@ UARTn Rx buffer, r0 = received char
	ldr		r1,[r12,#oUERSTAT]	@ Rx error status, r1 = error status
	mov		pc,lr								@ return

@	------------------------------------------

@ Send char to UART
@ char is in lsbyte of r0
@
@ Modified:	r0, r1, r12

tx_char_r0:
	and		r0,r0,#0xff					@ just the lsbyte
	mov		r12,#S3C24X0_UART_BASE
	add 	r12,r12,#0x8000

wt4txmt:	
	ldr		r1,[r12,#oUTRSTAT]	@ read UTRSTATn UART Tx/Rx status
	tst		r1,#4								@ bit 2 = Tx empty (Tx buffer & Tx shift reg)
	beq		wt4txmt							@ wait for Tx empty

	strb	r0,[r12,#oUTXH]			@ send the char
	mov		pc,lr								@ return

@	------------------------------------------

@ Transmit null-terminated string out UART
@ r0 points to start of string
@
@ Modified:	r0, r1, r2, r3, r12

snd_str:
	mov      r2,#0									@ char index = 0
	b        snd_str_entry

snd_str_lp:	
	mov      r12,#S3C24X0_UART_BASE
	add 		 r12,r12,#0x8000
	ldr      r3,[r12,#oUTRSTAT]			@ UTRSTATn = Tx/Rx status
	tst      r3,#2									@ bit 1 = Tx buffer empty
	beq      snd_str_lp							@ loop if not empty

	strb     r1,[r12,#oUTXH]				@ UTXHn = UARTn Tx hold
	add      r2,r2,#1								@ index++
snd_str_entry:
	ldrb     r1,[r0,r2]  						@ read char from string at index
	cmp      r1,#0									@ terminal null?
	bne      snd_str_lp							@ loop for next char
	bx       lr											@ return

@	------------------------------------------

snd_msg4:
	add			r0,pc,#0					@ r0 = here + 8 -> ""
	b				snd_str

	.asciz			"\r\nVbat="

@ *************************************************************************




/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */

@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	bad_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	ldr	r2, _armboot_start
	sub	r2, r2, #(CONFIG_STACKSIZE+CFG_MALLOC_LEN)
	sub	r2, r2, #(CFG_GBL_DATA_SIZE+8)  @ set base 2 words into abort stack
	ldmia	r2, {r2 - r3}			@ get pc, cpsr
	add	r0, sp, #S_FRAME_SIZE		@ restore sp_SVC

	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}			@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp
	.endm

	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	add     r7, sp, #S_PC
	stmdb   r7, {sp, lr}^                   @ Calling SP, LR
	str     lr, [r7, #0]                    @ Save calling PC
	mrs     r6, spsr
	str     r6, [r7, #4]                    @ Save CPSR
	str     r0, [r7, #8]                    @ Save OLD_R0
	mov	r0, sp
	.endm

	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4			@ return & move spsr_svc into cpsr
	.endm

	.macro get_bad_stack
	ldr	r13, _armboot_start		@ setup our mode stack
	sub	r13, r13, #(CONFIG_STACKSIZE+CFG_MALLOC_LEN)
	sub	r13, r13, #(CFG_GBL_DATA_SIZE+8) @ reserved a couple spots in abort stack

	str	lr, [r13]			@ save caller lr / spsr
	mrs	lr, spsr
	str     lr, [r13, #4]

	mov	r13, #MODE_SVC			@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13
	mov	lr, pc
	movs	pc, lr
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, FIQ_STACK_START
	.endm

/*
 * exception handlers
 */
	.align  5
undefined_instruction:
	get_bad_stack
	bad_save_user_regs
	bl 	do_undefined_instruction

	.align	5
software_interrupt:
	get_bad_stack
	bad_save_user_regs
	bl 	do_software_interrupt

	.align	5
prefetch_abort:
	get_bad_stack
	bad_save_user_regs
	bl 	do_prefetch_abort

	.align	5
data_abort:
	get_bad_stack
	bad_save_user_regs
	bl 	do_data_abort

	.align	5
not_used:
	get_bad_stack
	bad_save_user_regs
	bl 	do_not_used

#ifdef CONFIG_USE_IRQ

	.align	5
irq:
	get_irq_stack
	irq_save_user_regs
	bl 	do_irq
	irq_restore_user_regs

	.align	5
fiq:
	get_fiq_stack
	/* someone ought to write a more effiction fiq_save_user_regs */
	irq_save_user_regs
	bl 	do_fiq
	irq_restore_user_regs

#else

	.align	5
irq:
	get_bad_stack
	bad_save_user_regs
	bl 	do_irq

	.align	5
fiq:
	get_bad_stack
	bad_save_user_regs
	bl 	do_fiq

#endif
