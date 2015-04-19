/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * stdarg.h
 * Explorer/include/stdarg.h
 * 8/15/2014 12:15 AM
 */

#ifndef _STDARG_H_
#define _STDARG_H_

typedef char* va_list;

#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )					//��һ����ѡ������ַ
#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) ) 		//��һ��������ַ
#define va_end(ap) ( ap = (va_list)0 ) 										// ��ָ����Ϊ��Ч

#endif
