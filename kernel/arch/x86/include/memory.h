<<<<<<< HEAD
/**
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer memory management head
 *ExplorerOS/Kernel/include/memory.h
 *version:Alpha
 *7/9/2014 8:30 AM
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <types.h>

u32 *pt;
u32 *pdt;

void inti_memory(void);
void *oldkmalloc(size_t size);
void oldkfree(u32 addr, u32 size);
u32 kmap(u32 vir_addr, u32 phy_addr, u32 size);
void kmemcpy(u8 *source, u8 *object, u32 count);
int32 creat_pdt(u32 *point);
int32 copy_pdt(u32 *source_point, u32 *target_point);

=======
/**
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer memory management head
 *ExplorerOS/Kernel/include/memory.h
 *version:Alpha
 *7/9/2014 8:30 AM
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <types.h>

u32 *pt;
u32 *pdt;

void inti_memory(void);
void *oldkmalloc(size_t size);
void oldkfree(u32 addr, u32 size);
u32 kmap(u32 vir_addr, u32 phy_addr, u32 size);
void kmemcpy(u8 *source, u8 *object, u32 count);
int32 creat_pdt(u32 *point);
int32 copy_pdt(u32 *source_point, u32 *target_point);

>>>>>>> explorer/master
#endif