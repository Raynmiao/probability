/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
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

/*
 * CPU specific code
 */

#include "interrupts.h"

extern int bsp_int_disable(void);

void cpu_init()
{
}

/* 
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
uint32_t get_fp(void)
{
    uint32_t temp;
    __asm__ __volatile__(
			 "mov %0, fp"
			 : "=r" (temp)
			 :
			 : "memory");
    return temp;
}

void do_reset ()
{
    extern void reset_cpu(uint32_t addr);

    bsp_int_disable();
    reset_cpu(0);
    /*NOTREACHED*/
}

