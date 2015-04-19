/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer IDE Disk Support
 * Explorer/drivers/hdd.c
 * 2014.7.18 5:48 PM
 */
 
#include "hdd.h"
#include <lib/mem.h>
#include <types.h>
#include <stdlib.h>
#include <memory.h>

/**��ATA��׼�У�IDE�����30�����������10����ͨ��������*/
#define HD_CMD_READ		0x20			//��ȡ��������
#define HD_CMD_WRITE	0x30			//д����������
#define HD_CMD_CHECK	0x90			//�����������
	
/**��������ʱ��������������*/
#define MAX_ERRORS	10

/**ATӲ�̿������Ĵ����˿ڼ�����*/
/**��ʱ*/
#define HD_DATA			0x1f0
#define HD_ERROR		0x1f1
#define HD_NSECTOR		0x1f2
#define HD_SECTOR		0x1f3
#define HD_LCYL			0x1f4
#define HD_HCYL			0x1f5
#define HD_CURRENT		0x1f6
#define HD_STATUS		0x1f7
/**дʱ*/
#define HD_PRECOMP		0x1f1
#define HD_COMMAND		0x1f7

unsigned int LBA_start;


/**��ʼ�����̳�����Lab Explorer Developers<2322869088@qq.com>ʵ��
 * ���ܣ��������̿��������жϹ��������̵ķ������
 */
void init_hdd(void)
{
	/**�����жϿ��������жϴ������*/
	register_PIC(0xe, &int_HDC_handle, "Hard Disk Control");
	
	/**����һ�������Ŀռ�*/
	void *point;
	point = vmalloc(512);
	if (point == NULL) error("HDD:memory allocate error!");		/**����ʧ�ܿ���*/
	
	/**��ȡӲ��������Ϣ*/
	HD_get_phy_info(point);
	
	/**��ȡMBR����ȡ������Ϣ*/
	read_disk(0, (unsigned short int*) point, 1);
	memcpy(&LBA_start, (point + 0x1be + 8), 2);
	struct Master_Boot_Record *MBR;
	MBR = (struct Master_Boot_Record *) point;
	//printk("MBR's effect:%#x", (*MBR).valid);
	
	printk("LAB_start:%#X", LBA_start);
	
	/**�ͷ�����ռ�*/
	vfree(point);
}

void read_disk(u32 LBA, u16 *buffer, u32 number)
{
	/**����������������ڶ�ȡʱ�����������л�*/
	disable_schedule();
	u32 offset, i;
	io_out16(HD_NSECTOR,number);								/**����*/
	io_out8(HD_SECTOR,(LBA & 0xff));							/**LBA��ַ7~0*/
	io_out8(HD_LCYL,((LBA >> 8) & 0xff));						/**LBA��ַ15~8*/
	io_out8(HD_HCYL,((LBA >> 16) & 0xff));						/**LBA��ַ23~16*/
	io_out8(HD_CURRENT,(((LBA >> 24) & 0xff) + 0xe0));			/**LBA��ַ27~24 + LBAģʽ����Ӳ��*/
	io_out8(HD_STATUS,HD_CMD_READ);								/**������*/
	/**ѭ����DATA�˿ڶ�ȡ����*/
	for (i = 0; i != number; i ++)
	{
		hdd_wait();
		for (offset = 0; offset < 256; offset ++)
		{
			buffer[(i * 256) + offset] = io_in16(HD_DATA);		/**��DATA�˿��ж�ȡ����*/
		}
	}
	/**�������*/
	enable_schedule();
	return;
}

void hdd_wait(void)
{
	/**�ȴ�������ʱ*/
	//unsigned long n;
	for (; (io_in8(HD_STATUS) & 0x88) != 0x08;);				/**ѭ���ȴ�*/
}


/**���̲�����ȡ����*/
static void HD_get_phy_info(struct HD_info *info)
{
	
}


/**���̿������жϴ������*/
void int_HDC_handle(void)
{
	/**����Ŀǰ���ڴ��̿��������ж��޴����������ֻ�Ǽ������Ϣ*/
	printk("int HDC.\n");
	EOI();
}

