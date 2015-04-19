/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Standard Video Support Definition
 * Explorer/drivers/video.h
 * version:Alpha
 * 7/10/2014 8:30 AM
 */

#ifndef VEDIO_H_
#define VEDIO_H_

void init_VESA(void);
void refresh_scr(unsigned char *buffer);
void refresh_pix_32(unsigned long x, unsigned int long y, unsigned int color);

#endif

