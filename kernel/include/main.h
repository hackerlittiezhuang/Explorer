/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 function main definition
 * Explorer/include/main.h
 * 7/24/2014 10:56 AM
 */

#ifndef MAIN_H_
#define MAIN_H_

/**地址范围描述符结构体*/
#define ARDS_FREE	1
#define ARDS_RESV	2
struct Address_Range_Descriptor_Structure
{
	unsigned int BaseAddrLow;
	unsigned int BaseAddrHigh;
	unsigned int LengthLow;
	unsigned int LengthHigh;
	unsigned int Type;
};

#define BOOT_FLAG_MAX	4
#define BOOT_ARDS_NUM	8
/**启动信息结构体*/
struct boot_info
{
	/**信息一定要是"EBI"(Explorer Boot Information)*/
	char flag[BOOT_FLAG_MAX];
	/**启动信息长度*/
	unsigned int size;
	/**屏幕长度*/
	unsigned int video_length;
	/**屏幕宽度*/
	unsigned int video_width;
	/**屏幕颜色宽度*/
	unsigned int video_bpp;
	/**屏幕显存*/
	unsigned char *video_vram;
	/**内存分布信息*/
	struct Address_Range_Descriptor_Structure ARDS[BOOT_ARDS_NUM];
};


/**内核主函数*/
void main(struct boot_info *boot_info);


#endif
