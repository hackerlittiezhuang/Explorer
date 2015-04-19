/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Task Bar
 * Explorer/GUI/window/task_bar.c
 * version:Alpha
 * 2/5/2015 1:37 PM
 */

#include <GUI.h>
#include <time.h>
#include <studio.h>
#include <stdlib.h>
#include <lib/graphics.h>
#include "window.h"
#include "../layer.h"

/**windowϵͳʱ��ˢ�º���*/
void window_refresh_date(void)
{
	/**ʱ���ַ����ݻ�����*/
	char date_buf[15];
	
	/**�ػ�ʱ������*/
	GUI_put_square(taskbar_layer, TASKBAR_COLOR, (Video_Info.xres - 15 * 8), 1, 15 * 8, 19);			/**������*/
	
	/**�ַ�����ʽת��*/
	if (ms <= 500)
	{
		sprintf(date_buf, "%d:%02d %d/%d/%d", hour, min, mon, day, year);
	}else{
		sprintf(date_buf, "%d %02d %d/%d/%d", hour, min, mon, day, year);
	}
	
	/**���ʱ��*/
	GUI_put_string(taskbar_layer, TIME_COLOR, (Video_Info.xres - 15 * 8), 4, 0, 0, font("Standard Font"), date_buf);
}
