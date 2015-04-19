/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Time
 * Explorer/arch/x86/kernel/time.c
 * version:Aplha
 * 1/17/2014 9:52 PM
 */

#include "cmos.h"
#include <time.h>
#include <types.h>

/**Explorerʱ�����*/

/**ʱ����صı���*/
ulong century;					/**����*/
ulong year, mon, day;			/**�ꡢ�¡���*/
ulong hour, min, sec;			/**ʱ���֡���*/
ulong week_day;					/**һ���е�ǰ��*/
ulong ms = 0;					/**΢��*/
ulong system_runtime = 0;		/**ϵͳ����ʱ�䣬΢���ʱ��һ��ʼ��0*/

/**BCD��ת����16���Ʒ���*/
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf)

void init_time(void)
{
	/**��ȡCMOSоƬ�ϵ��ꡢ�¡��ա�ʱ���֡���*/
	sec = BCD_HEX(read_cmos(CMOS_CUR_SEC));			/**��ǰ��*/
	min = BCD_HEX(read_cmos(CMOS_CUR_MIN));			/**��ǰ��*/
	hour = BCD_HEX(read_cmos(CMOS_CUR_HOUR));		/**��ǰʱ*/
	week_day = BCD_HEX(read_cmos(CMOS_WEEK_DAY));	/**һ���е�ǰ��*/
	day = BCD_HEX(read_cmos(CMOS_MON_DAY));			/**һ���е�ǰ��*/
	mon = BCD_HEX(read_cmos(CMOS_CUR_MON));			/**��ǰ��*/
	year = BCD_HEX(read_cmos(CMOS_CUR_YEAR));		/**��ǰ��*/
	century = BCD_HEX(read_cmos(CMOS_CUR_CEN));		/**��ǰ����*/
	/**CMOS�е���ݽ����Ǵӱ����Ϳ�ʼ�����ڵľ���ֵ����Ҫ��������*/
	year += century * 100;
	printk("time:century:%d year:%d month:%d day:%d week:%d hour:%d min:%d sec:%d\n", century, year, mon, day, week_day, hour, min, sec);
}