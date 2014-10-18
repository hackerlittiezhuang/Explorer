<<<<<<< HEAD
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer function kmalloc head
 * Explorer/include/kmalloc.h
 * version:Alpha
 * 2/10/2014 9:04 AM
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_

#include <stddef.h>

/*����ڴ�ص��ڴ�����޶�*/
#define		NUMBER_OF_128KB 8
#define		NUMBER_OF_64KB 64
#define		NUMBER_OF_32KB 16
#define		NUMBER_OF_16KB 16
#define		NUMBER_OF_8KB 16
#define		NUMBER_OF_4KB 16
#define		NUMBER_OF_2KB 16
#define		NUMBER_OF_1KB 16
#define		NUMBER_OF_512B 16
#define		NUMBER_OF_256B 16
#define		NUMBER_OF_128B 16
#define		NUMBER_OF_64B 16
#define		NUMBER_OF_32B 16
#define		NUMBER_OF_16B 16

/**/
/**
 * �ڴ�ص�Ԫ�ṹ
 * mm.base:�ڴ����ַ
 * mm.attr:Ϊ��Ԫ���ԣ��յ�Ԫ��δ���䵥Ԫ���ѷ��䵥Ԫ��
 */
struct mm
{
	void *base;
	char attr;
};

/**
 * �ڴ�ص�Ԫ�ṹ������
 * mm.attr = 0:�ձ����ʹ�õı��
 * mm.attr = 1:�ɷ����ڴ�ı�����ã��п����ڴ棬û��ʹ�ã����Է��䣩
 * mm.attr = -1:���ɷ����ڴ�ı���Ѿ���ʹ�ã����ܷ��䣩
 */
#define		EMPTY_TABLE 0
#define		AVAILABLE_TABLE 1
#define		USED_TABLE -1

/*һ�����ڴ�ؽṹ*/
struct mem_pool
{
	/**�ɷ�����ڴ�����*/
	unsigned long available_128KB;
	unsigned long available_64KB;
	/**�ڴ������*/
	unsigned long num_128KB;
	unsigned long num_64KB;
	/**struct mm define in kmalloc.h*/
	struct mm mem_128KB[NUMBER_OF_128KB];
	struct mm mem_64KB[NUMBER_OF_64KB];
	struct mm mem_32KB[NUMBER_OF_32KB];
	struct mm mem_16KB[NUMBER_OF_16KB];
	struct mm mem_8KB[NUMBER_OF_8KB];
	struct mm mem_4KB[NUMBER_OF_4KB];
	struct mm mem_2KB[NUMBER_OF_2KB];
	struct mm mem_1KB[NUMBER_OF_1KB];
	struct mm mem_512B[NUMBER_OF_512B];
	struct mm mem_256B[NUMBER_OF_256B];
	struct mm mem_128B[NUMBER_OF_128B];
	struct mm mem_64B[NUMBER_OF_64B];
	struct mm mem_32B[NUMBER_OF_32B];
	struct mm mem_16B[NUMBER_OF_16B];
};

/**private function needn't use "extern "C{}"*/
static unsigned long alloc_128KB_to_64KB(void *point);

/**
 * use "extern "C{}"
 * If this file used by C++
 * and this function is global function
 */
#ifdef __cplusplus
extern "C" {
#endif

void init_kmalloc(void);
void *kmalloc(size_t size);
void kfree(void *point);

#ifdef __cplusplus
}
#endif

=======
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer function kmalloc head
 * Explorer/include/kmalloc.h
 * version:Alpha
 * 2/10/2014 9:04 AM
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_

#include <stddef.h>

/*����ڴ�ص��ڴ�����޶�*/
#define		NUMBER_OF_128KB 8
#define		NUMBER_OF_64KB 64
#define		NUMBER_OF_32KB 16
#define		NUMBER_OF_16KB 16
#define		NUMBER_OF_8KB 16
#define		NUMBER_OF_4KB 16
#define		NUMBER_OF_2KB 16
#define		NUMBER_OF_1KB 16
#define		NUMBER_OF_512B 16
#define		NUMBER_OF_256B 16
#define		NUMBER_OF_128B 16
#define		NUMBER_OF_64B 16
#define		NUMBER_OF_32B 16
#define		NUMBER_OF_16B 16

/**/
/**
 * �ڴ�ص�Ԫ�ṹ
 * mm.base:�ڴ����ַ
 * mm.attr:Ϊ��Ԫ���ԣ��յ�Ԫ��δ���䵥Ԫ���ѷ��䵥Ԫ��
 */
struct mm
{
	void *base;
	char attr;
};

/**
 * �ڴ�ص�Ԫ�ṹ������
 * mm.attr = 0:�ձ����ʹ�õı��
 * mm.attr = 1:�ɷ����ڴ�ı�����ã��п����ڴ棬û��ʹ�ã����Է��䣩
 * mm.attr = -1:���ɷ����ڴ�ı���Ѿ���ʹ�ã����ܷ��䣩
 */
#define		EMPTY_TABLE 0
#define		AVAILABLE_TABLE 1
#define		USED_TABLE -1

/*һ�����ڴ�ؽṹ*/
struct mem_pool
{
	/**�ɷ�����ڴ�����*/
	unsigned long available_128KB;
	unsigned long available_64KB;
	/**�ڴ������*/
	unsigned long num_128KB;
	unsigned long num_64KB;
	/**struct mm define in kmalloc.h*/
	struct mm mem_128KB[NUMBER_OF_128KB];
	struct mm mem_64KB[NUMBER_OF_64KB];
	struct mm mem_32KB[NUMBER_OF_32KB];
	struct mm mem_16KB[NUMBER_OF_16KB];
	struct mm mem_8KB[NUMBER_OF_8KB];
	struct mm mem_4KB[NUMBER_OF_4KB];
	struct mm mem_2KB[NUMBER_OF_2KB];
	struct mm mem_1KB[NUMBER_OF_1KB];
	struct mm mem_512B[NUMBER_OF_512B];
	struct mm mem_256B[NUMBER_OF_256B];
	struct mm mem_128B[NUMBER_OF_128B];
	struct mm mem_64B[NUMBER_OF_64B];
	struct mm mem_32B[NUMBER_OF_32B];
	struct mm mem_16B[NUMBER_OF_16B];
};

/**private function needn't use "extern "C{}"*/
static unsigned long alloc_128KB_to_64KB(void *point);

/**
 * use "extern "C{}"
 * If this file used by C++
 * and this function is global function
 */
#ifdef __cplusplus
extern "C" {
#endif

void init_kmalloc(void);
void *kmalloc(size_t size);
void kfree(void *point);

#ifdef __cplusplus
}
#endif

>>>>>>> explorer/master
#endif