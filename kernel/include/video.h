#ifndef VEDIO_H_
#define VEDIO_H_

unsigned char *vram;
unsigned int xsize;
unsigned int ysize;
unsigned int vram_len;
unsigned int bpp;

void inti_graph(void);
void refresh_scr(unsigned char *buffer);
void refresh_pix_32(unsigned long x, unsigned int long y, unsigned int color);

#endif

