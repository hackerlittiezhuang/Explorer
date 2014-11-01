/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer 8254 head
 *Explorer 0.01/arch/x86/include/i8254.h
 *version:1.1
 *8/27/2014 12:31 PM
 */

#ifndef PIT_H_
#define PIT_H_

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

#define CLOCK_TICK_RATE 1193180
/*set PIT work on 1000Hz per sec*/
#define HZ		1000
#define LATCH  ((CLOCK_TICK_RATE + HZ / 2) / HZ)

struct timer_task
{
	unsigned long long time_size;
	unsigned long long time;
	unsigned char state;
	void (*function)(void);
	struct timer_task *prev_task, *next_task;
};

extern int_PIT;

void init_PIT(void);
void int_PIT_display(void);
struct timer_task *settimer(void (*function)(void), unsigned long long time, unsigned char state);

#endif