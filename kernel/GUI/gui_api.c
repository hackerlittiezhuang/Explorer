/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface API
 * Explorer/GUI/gui_api.c
 * version:Alpha
 * 2/8/2015 11:41 PM
 */

#include <syscalls.h>
#include <GUI.h>
#include "window/window.h"

/**API�б�*/
#define NEW_WINDOW	1

/**GUI API���ú���*/
void sys_GUI_API(struct context context)
{
	/**���������*/
	disable_schedule();
	
	/**�ַ�����*/
	switch(context.ebx)
	{
		case NEW_WINDOW: goto new_window;
		default: goto finish;
	}
	
/**
 * �´������������
 * ebx = 1, ecx = ptr of title, edx = style, esi = length, edi = width
 * ����ֵ�����eax�У�Ϊ�´��ڵĽṹָ��
 */
new_window:

	/**���ó���*/
	context.eax = (int) GUI_window((char *)context.ecx, context.edx,0 , 0, context.esi, context.edi);
	goto finish;
	
/**����ϵͳ����*/
finish:
	/**�������*/
	enable_schedule();
	
	/**��������*/
	return;
}