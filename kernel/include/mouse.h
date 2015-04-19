/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu.1314@gmail.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Mouse support head
 * Explorer/include/mouse.h
 * version:Alpha
 * 8/26/2014 10:30 PM
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#define SIZE_OF_BUFFER_MOUSE	32

void init_mouse(void);
void enable_mouse(void);
void wait_KBC_sendready(void);
void put_mouse_x(long x);
long get_mouse_x(void);
void put_mouse_y(long y);
long get_mouse_y(void);
void int_mouse_handle(void);

#endif