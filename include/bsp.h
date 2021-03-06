/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 *
 * Copyright (c) Puhui Xiong <phuuix@163.com>
 * @file
 *   This header file is BSP's interface to os kernel .
 *
 * @History
 *   AUTHOR         DATE           NOTES
 */

#ifndef __D_BSP_H__
#define __D_BSP_H__

#include "inttypes.h"

/* internal memory range  */
#define MEMLOW 0x20000000
#define MEMHIGH 0x20020000

extern uint32_t sys_passed_ticks;
extern uint32_t sys_intdisable_nested;

#include "config.h"
#include "journal.h"

#ifdef INCLUDE_JOURNAL

/* disable interrupt and save cpu flags */
#define SYS_FSAVE(f) \
	do { \
		f = bsp_fsave(); \
		if(0 == sys_intdisable_nested) {\
			journal_interrupt(JOURNAL_TYPE_IRQDISABLE, 0); \
		} \
		sys_intdisable_nested ++; \
	} while(0)

/* restore cpu flags: 
 * TODO: if sysintdisable_nested < 0, a warning should be generated.
 */
#define SYS_FRESTORE(f) \
	do { \
		sys_intdisable_nested --; \
		if(0 == sys_intdisable_nested) {\
			journal_interrupt(JOURNAL_TYPE_IRQENABLE, 0); \
		} \
		bsp_frestore(f); \
	} while(0)

/* Using SYS_INT_DISABLE() and SYS_INT_ENABLE() is not encouraged */
#define SYS_INT_DISABLE() \
	do { \
		if (0 == sys_intdisable_nested) {\
			sys_intdisable_nested ++; \
			journal_interrupt(JOURNAL_TYPE_INTERRUPT, sys_intdisable_nested); \
			bsp_int_disable(); \
		} \
	} while(0)

/* enable interrupt directly
 * TODO: if sysintdisable_nested > 1, a warning should be generated.
 */
#define SYS_INT_ENABLE() \
	do { \
		if ( sys_intdisable_nested > 0) {\
			sys_intdisable_nested --; \
			journal_interrupt(JOURNAL_INT_ENABLE, sys_intdisable_nested); \
			bsp_int_enable(); \
		} \
	} while(0)

#else // INCLUDE_JOURNAL

#define SYS_FSAVE(f) \
	do { \
		f = bsp_fsave(); \
		sys_intdisable_nested ++; \
	} while(0)

#define SYS_FRESTORE(f) \
	do { \
		sys_intdisable_nested --; \
		f = bsp_frestore(); \
	} while(0)

#define SYS_INT_DISABLE() \
	do { \
		if (0 == sys_intdisable_nested) {\
			sys_intdisable_nested ++; \
			bsp_int_disable(); \
		} \
	} while(0)

#define SYS_INT_ENABLE() \
	do { \
		if ( sys_intdisable_nested > 0) {\
			sys_intdisable_nested --; \
			bsp_int_enable(); \
		} \
	} while(0)

#endif

#define KPRINTF(format, ...) \
do{ \
	uint32_t f; \
	SYS_FSAVE(f); \
	kprintf("%10d: ", sys_passed_ticks); \
	kprintf(format, ##__VA_ARGS__); \
	SYS_FRESTORE(f); \
}while(0)


uint32_t get_fp(void);
void dump_call_stack(uint32_t fp, uint32_t low, uint32_t high);


void panic(char *infostr);

/* kprintf */
int kprintf(char *fmt,...);

/* interrupt and exception interface */
void bsp_int_enable(void);

int bsp_int_disable(void);

uint32_t bsp_fsave(void);

void bsp_frestore(uint32_t flags);

void bsp_irq_mask(uint32_t irqno);

void bsp_irq_unmask(uint32_t irqno);

void bsp_isr_attach(uint16_t index, void (*handler)(int n));

void bsp_isr_detach(uint16_t index);

void bsp_esr_attach(uint16_t index, void (*handler)(int n));

void bsp_esr_detach(uint16_t index);

/* task support interface */
void bsp_task_switch(uint32_t from, uint32_t to);

void bsp_task_switch_interrupt(uint32_t from, uint32_t to);

char *bsp_task_init(int index, void (*task)(void *p), void *parm, char *stack, void (*killer)(void));

/* libc interface */
void bsp_gettime(uint32_t *tv_sec, uint32_t *tv_nsec);
void bsp_gettime_init();
uint32_t bsp_udelay_init();
void bsp_udelay(uint32_t us);



/* GDB remote debug stub interface */
void breakpoint();

void dump_buffer(uint8_t *buffer, uint16_t length);

/* other hardware and software interface */

#endif /* __D__BSP_H__ */

