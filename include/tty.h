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
/*   tty.h                                     0.3.0                     */
/*                                                                       */
/* COMPONENT                                                             */
/*   Kernel                                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*                                                                       */
/* CHANGELOG                                                             */
/*   AUTHOR         DATE                    NOTES                        */
/*   Bearix         2006-8-20               Version 0.3.0                */
/*************************************************************************/ 

#ifndef __D_TTY_H__
#define __D_TTY_H__

#include <sys/types.h>
#include <termios.h>
#include <ipc.h>

#define TTY_NAME_SIZE 8
#define TTY_RDBUF_SIZE 256
#define TTY_WTBUF_SIZE 256

/* tty flags */
#define TTY_FLAG_BUSY	0x80000000
#define TTY_FLAG_OPENED 0x40000000

struct tty_struct;

/* line discipline interface */
struct tty_ldisc
{
	int (*open)(struct tty_struct *tty);
	void (*close)(struct tty_struct *tty);
	ssize_t (*read)(struct tty_struct *tty, unsigned char *buf, size_t nr);
	ssize_t (*write)(struct tty_struct *tty, unsigned char *buf, size_t nr);
	int (*ioctl)(struct tty_struct *tty, unsigned int cmd, unsigned long arg);
};

/* tty driver interface */
struct tty_driver
{
	int (*open)(struct tty_struct *tty);
	void (*close)(struct tty_struct *tty);
	ssize_t (*read)(struct tty_struct *tty, unsigned char *buf, size_t nr);
	ssize_t (*write)(struct tty_struct *tty, unsigned char *buf, size_t nr);
	int (*ioctl)(struct tty_struct *tty, unsigned int cmd, unsigned long arg);
};

/* main tty_struct */
struct tty_struct
{
	/* fields initialized by register() */
	int flags;
	char name[TTY_NAME_SIZE];
	sem_t rdsem;
	mtx_t rdmtx;
	struct tty_ldisc *ldisc;
	struct tty_driver *driver;

	/* fields initialized by open() */
	struct termios termios;
	short rdpos;
	short wtpos;
	char rdbuf[TTY_RDBUF_SIZE];
	short linepos;
	char linebuf[TTY_RDBUF_SIZE];
};

struct tty_evmail
{
	char tty;
	char len;
	unsigned char data[6];
};

extern struct tty_struct systty[];

void tty_init();
int tty_register(char *name, struct tty_ldisc *ldisc, struct tty_driver *driver);
int tty_unregister(char *name);
int tty_post_mail(struct tty_evmail *mail);
struct tty_struct * tty_fw_open(char *name, int flags);
void tty_fw_close(struct tty_struct *tty);
ssize_t tty_fw_read(struct tty_struct *tty, unsigned char *buf, size_t nr);
ssize_t tty_fw_write(struct tty_struct *tty, unsigned char *buf, size_t nr);
int tty_fw_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);

#endif /* __D_TTY_H__ */
