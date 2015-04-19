/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management Unit
 * Explorer/arch/x86/kernel/memory.c
 * version:Alpha
 * 5/7/2014
 */

#include "../include/memory.h"
#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/**�ڴ���Ϣ*/
unsigned int all_mem = 0, real_mem = 0;

/**�ں������С*/
#define RANG_KERNEL_SIZE		268435456

/**�ں��ڴ��ֽ�ҳͼ*/
#define KER_MEM_BYTEMAP_PTR		0x100000
#define KER_MEM_BYTEMAP_SIZE	RANG_KERNEL_SIZE / 4096
unsigned char *ker_mem_bytemap = (unsigned char *) KER_MEM_BYTEMAP_PTR;

/**���������յ��ں�������ʼλ��*/
#define KERNEL_ALLOC_START		KER_MEM_BYTEMAP_PTR + KER_MEM_BYTEMAP_SIZE

/**�ں��ڴ��ֽ�ҳͼ��ÿ���ֵ�ĺ���*/
#define KER_MEM_BYTEMAP_FREE	0b00000000
#define KER_MEM_BYTEMAP_USED	0b00000001
#define KER_MEM_BYTEMAP_START	0b00000010
#define KER_MEM_BYTEMAP_END		0b00000100

/**�����ڴ�λҳͼ*/
#define PHY_MEM_BITMAP_PTR		0x30000
#define PHY_MEM_BITMAP_SIZE		(4294967296 / 4096) / 8
unsigned int *phy_mem_bitmap = (unsigned int *) PHY_MEM_BITMAP_PTR;

/**��ʼ���ڴ����Ԫ����*/
void init_MMU(struct boot_info *boot_info)
{
	unsigned long n;
	unsigned int BaseAddr, Length;
	
	/**��ӡ��Ϣ*/
	//printk("Register memory...\n");
	
	/**��ʼ�������ڴ�λҳͼ*/
	for (n = 0; n < (PHY_MEM_BITMAP_SIZE / sizeof(unsigned int)); n ++)
	{
		/**1�����Ѿ���ռ��*/
		phy_mem_bitmap[n] = 0xffffffff;
	}
	
	/**��ʼ���ں��ڴ��ֽ�ҳͼ*/
	for (n = 0; n < KER_MEM_BYTEMAP_SIZE; n ++)
	{
		ker_mem_bytemap[n] == KER_MEM_BYTEMAP_USED | KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_END;
	}
	
	/**ѭ����ȡAddress Range Descriptor Structure ��
	 * ���������ڴ�λҳͼ
	 */
	for (n = 0; n < BOOT_FLAG_MAX; n ++)
	{
		/**���ڴ����*/
		all_mem += boot_info->ARDS[n].LengthLow;
		
		/**�ж��Ƿ��Ǹ���4GB�ķ�Χ*/
		if (boot_info->ARDS[n].BaseAddrHigh != 0) break;
		
		/**4KB����*/
		boot_info->ARDS[n].BaseAddrLow = boot_info->ARDS[n].BaseAddrLow & 0xfffff000;
		boot_info->ARDS[n].LengthLow = boot_info->ARDS[n].LengthLow & 0xfffff000;
		
		/**�жϸ�ARDS�Ƿ����*/
		if (boot_info->ARDS[n].Type != ARDS_FREE) continue;
		
		/**�жϸ�ARDS�ķ�Χ�Ƿ�Ϊ0*/
		if (boot_info->ARDS[n].LengthLow == 0) continue;
		
		/**������Ϣ*/
		BaseAddr = boot_info->ARDS[n].BaseAddrLow;
		Length = boot_info->ARDS[n].LengthLow;
		real_mem += Length;
		
		/**��ӡ�÷�Χ��Ϣ*/
		//printk("available memory:%#x~%#x.\n", BaseAddr, BaseAddr + Length);
		
		/**������Ӧ�������ڴ�λҳͼ���ں��ڴ��ֽ�ҳͼ*/
		while (Length != 0)
		{
			/**���ÿ���*/
			set_phy_page_free(BaseAddr);
			
			/**�ж��Ƿ����ں��ڴ�����*/
			if (BaseAddr < RANG_KERNEL_SIZE)
			{
				set_ker_bytemap(BaseAddr, KER_MEM_BYTEMAP_FREE);
			}
			
			/**��������*/
			BaseAddr += 0x1000;
			Length -= 0x1000;
		}
	}
	
	/**��ӡ��Ϣ*/
	//printk("Register memory done.\n");
	//printk("Installed memory(RAM):%dMB(%dMB is available).\n", all_mem / 1048576, real_mem / 1048576);
	
	/**����256MB�����������һ����ʼ��*/
	if (all_mem < 268435456)
	{
		printk("Not enough memory!Please make sure the memory more than 256MB.");
		reset();
	}
	
	/**��������*/
	return;
}

/**�ڴ���������Ϣ�������*/
void output_mem_info(struct boot_info *boot_info)
{
	/**��ӡ�ڴ�ֲ���Ϣ*/
	unsigned long n;
	printk("Address Range Descriptor Structure:\nBaseAddrLow   BaseAddrHigh  LengthLow     LengthHigh    Type\n");
	for (n = 0; n < BOOT_ARDS_NUM; n++)
	{
		printk("%#010x    %#010x    %#010x    %#010x    %#010x\n", boot_info->ARDS[n].BaseAddrLow, boot_info->ARDS[n].BaseAddrHigh, boot_info->ARDS[n].LengthLow, boot_info->ARDS[n].LengthHigh, boot_info->ARDS[n].Type);
	}
	
	/**����Ϣ*/
	printk("Installed memory(RAM):%dMB(%dKB is available).\n", all_mem / 1048576, real_mem / 1024);
}

/**����ҳʹ�ú���*/
void set_phy_page_used(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] | (1 << ((ptr >> 12) & 0b11111));
}

/**����ҳ���ɺ���*/
void set_phy_page_free(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] & ~(1 << ((ptr >> 12) & 0b11111));
}

/**�����ں��ڴ��ֽ�ҳͼ����*/
void set_ker_bytemap(unsigned long ptr, char flag)
{
	ker_mem_bytemap[ptr >> 12] = flag;
}

/**��ȡһ������ҳ����*/
void *get_free_page(void)
{
	
}

/**�ͷ�һ������ҳ*/
void free_page(void *addr)
{
}

/**����ڴ���亯��*/
void *vmalloc(size_t size)
{
	/**�������*/
	io_cli();
	
	/**�жϳ����Ƿ�Ϊ0*/
	if (size == 0) error("allocate is error!");
	
	unsigned long n = (KERNEL_ALLOC_START >> 12), l;
	
	/**�Բ�������*/
	if ((size & 0xfff) != 0) size += 0x1000;
	size = size & 0xfffff000;
	
/**nΪ�����жϵĿ��п����ҳ��lΪ�����жϵ�ҳ*/
	/**Ѱ���㹻�����ڴ�����*/
	for (;;)
	{
		/**Ѱ�ҵ���������*/
		if (ker_mem_bytemap[n] == KER_MEM_BYTEMAP_FREE)
		{
			/**ѭ�����ÿ��п鳤��*/
			for (l = n;;l ++)
			{
				/**���������п鵽����Ϊֹ�Ҳ�����С*/
				if (ker_mem_bytemap[l] != KER_MEM_BYTEMAP_FREE)
				{
					/**���ز��Ҵ���һ��ҳ��ʼ��*/
					n = l + 1;
					break;
				}
				
				/**������ȷ���Ҫ��*/
				if ((l - n + 1) == (size >> 12))
				{
					/**���Ⱥ�����ת������ģ��*/
					goto allocate;
				}
			}
			
		}
		
		/**Ѱ����һ���ڴ��*/
		n ++;
		
		/**�ж�Ѱ���Ƿ񳬹��ں��ڴ�����*/
		if (n >= KER_MEM_BYTEMAP_SIZE)
		{
			/**�����������ʧ��*/
			return NULL;
		}
		
		/**�ж�ʣ���ں��ڴ�鳤���Ƿ�����Ҫ��*/
		if ((KER_MEM_BYTEMAP_SIZE - n) < (size >> 12))
		{
			return NULL;
		}
	}
	
/**����ģ�飬��ģ�齫����ڴ������Ϊ�Ѿ�ʹ��*/
/**nΪ���õ������ڴ���׵�ַ��lΪƫ��*/
allocate:
	/**���ȶ���λ�����ر���*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_USED;
	ker_mem_bytemap[n + (size >> 12) - 1] = ker_mem_bytemap[n + (size >> 12) - 1] | KER_MEM_BYTEMAP_END | KER_MEM_BYTEMAP_USED;
	
	/**������ڴ�鶼���Ϊռ��*/
	for (l = n; (l - n + 1) != (size >> 12); l ++)
	{
		ker_mem_bytemap[l] = ker_mem_bytemap[l] | KER_MEM_BYTEMAP_USED;
	}
	
	/**�������*/
	io_sti();
	
	/**��������*/
	return (void *) (n << 12);
}


/**����ڴ��ͷź���*/
void vfree(void *addr)
{
	/**�жϸõ�ַ�Ƿ�Ϊ�����������ڴ����׵�ַ*/
	if ((ker_mem_bytemap[(unsigned long) addr >> 12] & KER_MEM_BYTEMAP_START) != KER_MEM_BYTEMAP_START)
	{
		/**���������Ϣ*/
		printk("free the memory is error: this address is not the head of the block\n");
	}
	unsigned long n;
	
	/**ѭ�������ڴ��*/
	for (n = ((unsigned long) addr >> 12); ((ker_mem_bytemap[n] & KER_MEM_BYTEMAP_END) != KER_MEM_BYTEMAP_END); n ++)
	{
		/**�����ڴ��*/
		ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	}
	
	/**����ĩβ���ڴ��*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	
	/**�����˳�*/
	return;
}

