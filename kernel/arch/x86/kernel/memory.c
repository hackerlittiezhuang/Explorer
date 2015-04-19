/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management Unit
 * Explorer/arch/x86/kernel/memory.c
 * version:Alpha
 * 5/7/2014
 */
 
#include <memory.h>
#include <main.h>
#include <stddef.h>
#include <stdlib.h>
#include <lib/mem.h>
#include <../include/page.h>

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
#define KER_MEM_BYTEMAP_FREE	0b00000000			// ����ҳ
#define KER_MEM_BYTEMAP_USED	0b00000001			// �Ѿ�ʹ�õ��ڴ��
#define KER_MEM_BYTEMAP_START	0b00000010			// �Ѿ�ʹ�õ��ڴ�����ʼҳ
#define KER_MEM_BYTEMAP_END		0b00000100			// �Ѿ�ʹ�õ��ڴ���ĩβҳ

/**�����ڴ�λҳͼ*/
#define PHY_MEM_ALLOC_START		RANG_KERNEL_SIZE
#define PHY_MEM_BITMAP_PTR		0x30000
#define PHY_MEM_BITMAP_SIZE		(4294967296 / PAGE_SIZE) / 8
unsigned int *phy_mem_bitmap = (unsigned int *) PHY_MEM_BITMAP_PTR;


/**NOTICE��phy_mem_bitmap�Ǹ�32λ����ָ�룬����phy_mem_bitmap��Ϊ���飬ÿһ��Ԫ�ض��ܱ�ʾ32��ҳ����2^5*/


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

/**��ʼ���ڴ����Ԫ����*/
void init_MMU(struct boot_info *boot_info)
{
	unsigned long n;
	unsigned int BaseAddr, Length;
	
	/**�ں������С����Ϊ4MB��������*/
	if ((RANG_KERNEL_SIZE % 4194304) != 0) reset();
	
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
	for (n = 0; n < BOOT_ARDS_NUM; n ++)
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
			BaseAddr += PAGE_SIZE;
			Length -= PAGE_SIZE;
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

/**ҳĿ¼��*/
unsigned long *pdt, *pt;

/**��ʼ����ҳģʽ����*/
void init_paging(void)
{
	unsigned long ptr;
	
	/**����ҳĿ¼��*/
	for (pdt = NULL; pdt == NULL; )
		pdt = vmalloc(PAGE_SIZE);
	
	/**����ҳ��*/
	for (pt = NULL; pt == NULL; )
		pt = vmalloc((RANG_KERNEL_SIZE / PAGE_SIZE) * sizeof(pt));
	
	/**������ҳ��ע�ᵽҳĿ¼��*/
	for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	{
		pdt[ptr] = (ptr * PAGE_SIZE) + (int)pt + 0x7;
	}
	
	/**�������ڴ������ҳ��ע�ᵽҳ����*/
	for (ptr = 0; ptr < RANG_KERNEL_SIZE / 4096; ptr ++)
	{
		pt[ptr] = ptr * PAGE_SIZE + 0x7;
	}
	
	/**�����ҳģʽ*/
	goto_paging(pdt);
}

/**����ռ�ӳ�亯��*/
unsigned int kmap(unsigned int vir_addr, unsigned int phy_addr, unsigned int size)
{
	/**��������ַ�������ַ��4KB���룬����󷵻�*/
	if ((vir_addr & 0xfff) != 0) return 1;
	if ((phy_addr & 0xfff) != 0) return 2;
	
	/**���ӳ�䳤�ȷ�4KB���룬Ҳ���󷵻�*/
	if ((size & 0xfff) != 0) return 3;

	for (;size != 0;size -= PAGE_SIZE)
	{
		pt[(vir_addr >> 12)] = phy_addr + 0x7;
		phy_addr += PAGE_SIZE;
		vir_addr += PAGE_SIZE;
	}
	
	/**��������*/
	return 0;
}

/**������ҳĿ¼����*/
unsigned long new_pdt(void)
{
	unsigned long ptr;
	unsigned long *new_pdt;
	
	/**�����ڴ洴���µ�ҳĿ¼��*/
	for (new_pdt = NULL; new_pdt == NULL; )
		new_pdt = vmalloc(PAGE_SIZE);
	
	/**���Ѿ�������ҳĿ¼�������ҳ����������ҳĿ¼����*/
	for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	{
		new_pdt[ptr] = pdt[ptr];
	}
}

/**��ȡһ������ҳ����
 * ����ֵ��NULL�����ȡ��������ҳʧ�ܣ���NULL�����ȡ�ɹ���
 */
void *get_free_page(void)
{
	unsigned long n, n2, new_page;
	unsigned int bitmap;
	
	/**���ж�phy_mem_bitmap�������Ƿ��и�32λԪ�ز�Ϊ0xffffffff*/
	// for (n = 0; n < PHY_MEM_BITMAP_SIZE / sizeof(unsigned int); n ++)
	for (n = PHY_MEM_ALLOC_START / (PAGE_SIZE * 32); n < PHY_MEM_BITMAP_SIZE / sizeof(unsigned int); n ++)
	{
		if (phy_mem_bitmap[n] != 0xffffffff)
		{
			/**���ĳ��Ԫ���б�ʾ��ҳ�����п��е�ҳ�������Ѱ��*/
			bitmap = phy_mem_bitmap[n];
			
			/**ѭ�������ҳ�����ĸ�ҳ�ǿ��е�*/
			for (n2 = 0; n2 < 32; n2 ++)
			{
				/**�����ж���Ӧ������λ�Ƿ�Ϊ0*/
				if (((bitmap >> n2) & 1) == 0)
				{
					/**���ҳ��ʵ�ʵ�ַ�������*/
					new_page = (n * 32 * PAGE_SIZE) + (n2 * PAGE_SIZE);
					
					/**�������ҳΪռ��*/
					set_phy_page_used(new_page);
					
					/**�������ҳ*/
					return (void *)new_page;
				}
			}
		}
	}
	
	/**�����е������ʱ�򣬴��������ҳλͼȴ�޺��ʵ�ҳ����ʱֻ�ܷ���NULLֵ���Ժ����������ʵ��ҳ����*/
	
	/**��Ч����*/
	return NULL;
}

/**�ͷ�һ������ҳ����*/
void free_page(void *addr)
{
	/**����ҳ����Ϊ����*/
	set_phy_page_free((unsigned long)addr);
}

/**����ڴ���亯��*/
void *vmalloc(size_t size)
{
	/**ֹͣ����*/
	disable_schedule();
	
	/**�жϳ����Ƿ�Ϊ0*/
	if (size == 0)
	{
		/**�������*/
		enable_schedule();
		return NULL;
	}
	
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
			/**�������*/
			enable_schedule();
			/**�����������ʧ��*/
			return NULL;
		}
		
		/**�ж�ʣ���ں��ڴ�鳤���Ƿ�����Ҫ��*/
		if ((KER_MEM_BYTEMAP_SIZE - n) < (size >> 12))
		{
			/**�������*/
			enable_schedule();
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
	enable_schedule();
	
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

