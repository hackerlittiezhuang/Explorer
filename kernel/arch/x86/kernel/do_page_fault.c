/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer ҳ���ϴ������
 * Explorer/arch/x86/kernel/do_page_fault.c
 * version:Alpha
 * 8/7/2014 10:46 AM
 */

#include <lib/mem.h>
#include "../include/page.h"
#include <stdlib.h>
#include <memory.h>
#include <types.h>

int rentry = 0;

/**ҳ���ϴ�����*/
void do_page_fault(int error_code)
{
	u32 cr2, *pdt, *pt, *new_page;
	
	/**�ж��Ƿ�����*/
	if (rentry != 0)
	{
		printk("rentry!!!");
		fin:goto fin;
	}else{
		rentry = 1;
	}
	
	/**��ȡCR2��Ϣ*/
	cr2 = read_CR2();
	pdt = (u32 *) (read_CR3() & 0xfffff000);
	
		
	/**�ж��Ƿ���ȱҳ�������ж�*/
	if ((error_code == 2) | (error_code == 0))
	{
		printk("do page fault address:%#X.\n", cr2);
		
		/**�������ԭ����û��ҳ��*/
		if ((pdt[(cr2 >> 22)] & 1) == 0)
		{
			/**��ȡһ����ҳ*/
			for (new_page = NULL; new_page == NULL; )
				new_page = vmalloc(PAGE_SIZE);
			
			/**������ҳ��Ϊҳ��ҳĿ¼���ҳ������ں�̬�ռ��У�*/
			pdt[(cr2 >> 22)] = ((unsigned int)new_page | 0x7);
			
			/**������ҳ����֤������ָ���*/
			memset((u8 *) (pdt[(cr2 >> 22)]), 0x00, ((4096 - 256) / 4));
		}
		
		/**�Ȼ�ȡҳ���ַ*/
		pt = (u32 *)(pdt[(cr2 >> 22)] & 0xfffff000);
		
		/**�����Ƿ����ȱҳ��������һ��ȱ��ҳ*/
		pt[(cr2 & 0x3FF000) >> 12] = (get_free_page() | 0x7);
		
		/*��ʾ��Ϣ������*/
		//printk("Page fault:allocated.%X",pdt[(cr2 >> 22)]);
		
		goto finish;
	}else{
		/**����shell*/
		enable_shell();
		
		/**�ر�shell*/
		printk("Page fault:(Unknown)error code:0x%X", error_code);
		
		/**ͣ��ָ��*/
		io_hlt();
	}
	
finish:
	rentry = 0;
	return;
}


