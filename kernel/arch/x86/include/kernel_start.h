/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer initialization in the start of kernel
 *Explorer 0.01/arch/x86/include/kernel_start.h
 *version:Alpha
 *8/26/2014 11:19 PM
 */

#ifndef KERNEL_START_H_
#define KERNEL_START_H_

void kernel_start(void);
/*initialization device which we need*/
void init_dev(void);
void init_CPU(void);
void output_CPU_info(void);

#define		FAMILY ((CPUID_model & 0xF00) >> 8)
#define		EXTENDED_FAMILY ((CPUID_model & 0xFF00000) >> 20)

#define		MODEL ((CPUID_model & 0xF0) >> 4)
#define		EXTENDED_MODEL ((CPUID_model & 0xF0000) >> 16)

#endif