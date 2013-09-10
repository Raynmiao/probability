/*************************************************************************/
/* The Dooloo kernel                                                     */
/* Copyright (C) 2004-2006 Xiong Puhui (Bearix)                          */
/* All Rights Reserved.                                                  */
/*                                                                       */
/* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  */
/* THE PROPERTY OF DOOLOO RTOS DEVELOPMENT TEAM                          */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE                                       VERSION                    */
/*   gdb_cmd.c                                    0.3.0                  */
/*                                                                       */
/* COMPONENT                                                             */
/*   Shell                                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*                                                                       */
/* CHANGELOG                                                             */
/*   AUTHOR         DATE                    NOTES                        */
/*   Bearix         2006-8-20               Version 0.3.0                */
/*************************************************************************/ 

#include <sys/types.h>
#include <shell.h>
#include <arpa/inet.h>

extern void breakpoint();
extern void gdb_udp_ipaddr_set(uint32_t ip);
extern uint32_t gdb_udp_ipaddr_get();

/*
 * gdb trap command
 */
int gdb_trap(struct shell_session *ss, int argc, char **argv)
{
	breakpoint();

	return ROK;
}

#if 0
/* a temp inet_aton() and inet_addr() */
#include <net/in.h>
#include <ctype.h>
int inet_aton(cp, inp)
const char *cp;
struct in_addr *inp;
{
	unsigned long addr;
	int value;
	int part;

	if (!inp)
		return 0;

	addr = 0;
	for (part = 1; part <= 4; part++) {

		if (!isdigit(*cp))
			return 0;

		value = 0;
		while (isdigit(*cp)) {
			value *= 10;
			value += *cp++ - '0';
			if (value > 255)
				return 0;
		}

		if (part < 4) {
			if (*cp++ != '.')
				return 0;
		} else {
			char c = *cp++;
			if (c != '\0' && !isspace(c))
			return 0;
		}

		addr <<= 8;
		addr |= value;
	}

	inp->s_addr = htonl(addr);

	return 1;
}

unsigned long inet_addr(const char *cp)
{
	struct in_addr a;

	if (!inet_aton(cp, &a))
		return -1;
	else
		return a.s_addr;
}
#endif /* 0 */

/*
 * set gdb ip address
 */
int gdb_ipaddr_set(struct shell_session *ss, int argc, char **argv)
{
	char *paddr;
	uint32_t ip;

	paddr = argv[3];
	ip = inet_addr(paddr);
	if(ip != -1)
		gdb_udp_ipaddr_set(ntohl(ip));
	else
		ss->output("Invalid IP address.\n");
	
	return ROK;
}

/*
 * show gdb ip address
 */
int gdb_ipaddr_show(struct shell_session *ss, int argc, char **argv)
{
	uint32_t ip;
	uint8_t *p;

	ip = htonl(gdb_udp_ipaddr_get());
	p = (uint8_t *)&ip;
	ss->output("gdb ip address: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
	return ROK;
}

