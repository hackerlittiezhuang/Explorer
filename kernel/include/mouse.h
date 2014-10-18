/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu.1314@gmail.com>
 *Explorer Mouse support head
 *ExplorerOS/Kernel/include/mouse.h
 *version:Alpha
 *8/26/2014 10:30 PM
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0X47
void init_mouse(void);
void enable_mouse(void);
void wait_KBC_sendready(void);
void int_mouse_display(void);
#endif