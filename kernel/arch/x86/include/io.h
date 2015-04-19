/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 Intel x86�ܹ��������ָ������ܶ���
 * Explorer/arch/x86/include/io.h
 * 7/26/2014 7:05 PM
 */

#ifndef IO_H_
#define IO_H_

#include <types.h>

/**ͣ������
 * �ú���ִ��ʵ���ǻ��ָ��hlt
 * ִ�иú����󣬼������һֱ����ͣ��״̬(�͹���)��
 * ֱ����Ӳ���жϵķ���
 */
void io_hlt(void);


/**�رմ��ж�λ����
 * ����������ִ��ʵ���ǻ��ָ��cli��sti
 * io_cli����ִ�к󣬻ὫPSW�Ĵ������ж�λ��0���ر�Ӳ���ж�
 * io_sti����ִ�к󣬻ὫPSW�Ĵ������ж�λ��1������Ӳ���ж�
 */
void io_cli(void);
void io_sti(void);


/**��ȡ�ָ���־�Ĵ�������*/
ulong io_read_PSW();
void io_restore_PSW(ulong value);


/**���뺯��*/
u8 io_in8(u32 port);
u16 io_in16(u32 port);
u32 io_in32(u32 port);


/**�������*/
void io_out8(u32 port, u8 data);
void io_out16(u32 port, u8 data);
void io_out32(u32 port, u8 data);

#endif
