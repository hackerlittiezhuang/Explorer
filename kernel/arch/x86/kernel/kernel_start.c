/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Rose<roselyu@163.com> Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer initialization in the start of kernel
 * Explorer 0.01/arch/x86/kernel/kernel_start.c
 * version:Alpha
 * 9/14/2014 12:30 PM
 */

#include <lib/string.h>
#include <stdbool.h>
#include <font.h>
#include "../include/kernel_start.h"
/**This file including some of CPU family or model string*/
#include "../include/CPU.h"

void kernel_start(void)
{
	init_arch();
	init_memory();
	init_trap();
	init_task();
	init_dev();
	//Display Initialization
	init_font();
	init_graph();
	init_shell();
	//fin:goto fin;
}

/*initialization device which we need*/
void init_dev(void)
{
	init_CPU();
	init_PIC();
	init_PIT();
	init_keyboard();
}

bool support_CPUID;
unsigned int CPU_MAXID;
unsigned int CPU_EMAXID;
unsigned char CPU_OEM[13];
unsigned int CPUID_model;
unsigned int display_family;
unsigned int display_model;
unsigned char *CPU_family;
unsigned char *CPU_model;

void init_CPU(void)
{
	/**
	 *We can make sure this CPU is Intel 80386,
	 *If this CPU no support CPUID instruction.
	 *else this CPU is Intel 80486 or after.
	 *The method of check CPU if it can support CPUID
	 *is save Register EFLAGS and try to change the bit 21
	 *in Register EFLAGS, than compare if we write successful.
	 */
	

/*****************************WARING:can't add printk here*****************************/
/*****************************PLEASE ADD PRINTK TO output_CPU_info*****************************/
	/**
	 *get CPU OEM & CPUID MAX support
	 *CPUID.00H:EBX:EDX:ECX is string of OEM.
	 *CPUID.00H:EAX is MAX number basic CPUID support from CPU.
	 */
	CPU_OEM[12] = 0x00;
	asm volatile(" \
		xor		%eax,%eax; \
		cpuid; \
		movl	%eax,(CPU_MAXID); \
		movl	%ebx,(CPU_OEM); \
		movl	%edx,(CPU_OEM+4); \
		movl	%ecx,(CPU_OEM+8); \
	");
	
	/**
	 *get extended MAX value of CPUID instruction
	 */
	asm volatile(" \
		movl	$0x80000000,%eax; \
		cpuid; \
		movl	%eax,(CPU_EMAXID); \
	");
	
	/**
	 *get family & model
	 */


	
	if (strncmp(CPU_OEM, "GenuineIntel", 12) == 0)
	{
		asm volatile(" \
			movl	$0x01,%eax; \
			cpuid; \
			movl	%eax,(CPUID_model); \
		");
		if (FAMILY == 0x0f)
		{
			display_family = EXTENDED_FAMILY + FAMILY;
		}else{
			display_family = FAMILY;
		}
		
		if (FAMILY == 0x6 || FAMILY == 0xf)
		{
			display_model = (EXTENDED_MODEL << 4) + MODEL;
		}else{
			display_model = MODEL;
		}
		
		/**
		 *CPU family & model information
		 */
		if (display_family == 0x6)
		{
			CPU_family = CPU_family_06H;
			CPU_model = CPU_model_Unknown;
			
		}else if (display_family == 0xf)
		{
			CPU_family = CPU_family_0FH;
			CPU_model = CPU_model_Unknown;
		}else{
			CPU_family = CPU_family_Unknown;
			CPU_model = CPU_model_Unknown;
		}
	}else if (strncmp(CPU_OEM, "AuthenticAMD", 12) == 0)
	{
		CPU_family = CPU_family_AMD;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "Microsoft Hv", 12) == 0)
	{
		CPU_family = CPU_family_vpc;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "VMwareVMware", 12) == 0)
	{
		CPU_family = CPU_family_VMware;
		CPU_model = CPU_model_Unknown;
	}else
	{
		CPU_family = CPU_family_Unknown;
		CPU_model = CPU_model_Unknown;	
	}
}

void output_CPU_info(void)
{
	unsigned int brands[4]; //Ã¿´ÎµÄeax¡¢ebx¡¢ecx¡¢edx
	unsigned int i;

	printk("\n##CPU Information: ");
        for (i = 0x80000002; i <= 0x80000004; i++) {
            asm volatile ("cpuid"
                      : "=a"(brands[0]), "=b"(brands[1]), "=c"(brands[2]), "=d"(brands[3])
                      : "0" (i)
                      );
            printk("%s", (char *)brands);
        }
printk("\n\n");
	printk("CPU's manufacturer ID:%s\n", &CPU_OEM);
	printk("CPUID support in MAX:0x%X,extended CPUID support in MAX:0x%X.\n", CPU_MAXID, CPU_EMAXID);
	printk("CPU family:%s[0x%X].\n", CPU_family,display_family, CPU_model, display_model);
}