/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer ҳ���ϴ������
 * Explorer/arch/x86/kernel/do_page_fault.c
 * version:Alpha
 * 8/7/2014 10:46 AM
 */

#include <lib/mem.h>
#include "../include/memory.h"
#include <types.h>

void do_page_fault(int error_code)
{
	printk(".");
	u32 cr2, *pdt_address, old_cr3;
	/*��ȡ��Ϣ*/
	cr2 = read_CR2();
	pdt_address = (u32 *) (read_CR3() & 0xfffff000);
	/*�����ں˵Ĺ���ҳĿ¼��*/
	old_cr3 = read_CR3();
	//write_CR3(pdt);
	if ((error_code == 2) | (error_code == 0))/*��ȱҳ�������ж�*/
	{
		if ((pdt_address[(cr2 >> 22)] & 1) == 0)/*���û��ҳ��*/
		{
			pdt_address[(cr2 >> 22)] = (get_free_page() | 0x7);
			/*������ҳ*/
			memset((u8 *) (pdt_address[(cr2 >> 22)]), 0x00, ((4096 - 256) / 4));/*�Ժ�����ڴ����������0*/
		}
		/*һ��ȱ��ҳ*/
		((u32 *) (pdt_address[(cr2 >> 22)] & 0xfffff000))[(cr2 & 0x3FF000) >> 12] = (get_free_page() | 0x7);
		/*��ʾ��Ϣ������*/
		//printk("Page fault:allocated.%X",pdt_address[(cr2 >> 22)]);
		write_CR3(old_cr3);
		return;
	}else{
	printk("Page fault:(Unknown)error code:0x%X", error_code);
	io_hlt();
	}
}


