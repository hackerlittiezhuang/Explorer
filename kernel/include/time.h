/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Time Definition
 * Explorer/arch/x86/kernel/time.h
 * version:Aplha
 * 1/18/2014 12:12 PM
 */

#ifndef TIME_H_
#define TIME_H_

#include <types.h>

/**ʱ����صı���*/
extern ulong century;					/**����*/
extern ulong year, mon, day;			/**�ꡢ�¡���*/
extern ulong hour, min, sec;			/**ʱ���֡���*/
extern ulong week_day;					/**һ���е�ǰ��*/
extern ulong ms;							/**΢��*/
extern ulong system_runtime;				/**ϵͳ����ʱ�䣬һ��ʼ��0*/
	
#endif