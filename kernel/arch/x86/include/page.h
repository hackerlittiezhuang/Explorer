/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Paging Support
 * Explorer/arch/x86/include/page.h
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

#ifndef PAGE_H_
#define PAGE_H_

#define PAGE_SIZE	4096				// ҳ�Ĵ�С��4096Byte

/**ҳ���ϴ������*/
void do_page_fault(int error_code);

#endif