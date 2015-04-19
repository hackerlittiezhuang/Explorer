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
#include <stdbool.h>
#include <lib/graphics.h>
#include "window.h"
#include "../layer.h"

/**�������Ĺ滮*/
/**Application	Runing_window	Date*/

/**Application���򳤶�*/
unsigned long window_Application_length = 11 * 8 + 8;

/**Date���򳤶�*/
unsigned long window_Date_length = 15 * 8;

/**windowϵͳʱ��ˢ�º���*/
void window_refresh_date(void)
{
	/**ʱ���ַ����ݻ�����*/
	char date_buf[15];
	
	/**�ػ�ʱ������*/
	GUI_put_square(taskbar_layer, TASKBAR_COLOR, (Video_Info.xres - window_Date_length), 1, window_Date_length, TASKBAR_WIDTH - 1);			/**Date region*/
	
	/**�ַ�����ʽת��*/
	if (ms <= 500)					// ������ݵ�ǰ��΢���жϣ���0.5��Ϊ���ޣ�С��0.5����ʾ":"������0.5����ʾ" "�����Դﵽһ������˸һ�ε�Ч��
	{
		sprintf(date_buf, "%d:%02d %d/%d/%d", hour, min, mon, day, year);
	}else{
		sprintf(date_buf, "%d %02d %d/%d/%d", hour, min, mon, day, year);
	}
	
	/**���ʱ��*/
	GUI_put_string(taskbar_layer, TIME_COLOR, (Video_Info.xres - window_Date_length), 4, 0, 0, font("Standard Font"), date_buf);
}

/**Menu����������*/
struct window_menu_one
{
	char *text;						// ������ƣ�������ʾ��
	char *filename;					// ����صĺ���������ִ�У�
};

/**Menu��������*/
#define WINDOW_NUM_MENU		4
struct window_menu_one window_menu_lib[WINDOW_NUM_MENU] =
{
	{"Application1", "PROGRAM1.BIN"},
	{"Application2", "PROGRAM2.BIN"},
	{"Application3", "PROGRAM3.BIN"},
	{"Application4", "PROGRAM4.BIN"}
};

/**Menu�������ɫ*/
#define WINDOW_MENU_COLOR		TASKBAR_COLOR
#define WINDOW_MENU_LINE_COLOR	TASKBAR_LINE_COLOR

/**Menu����ĳ���*/
#define WINDOW_MENU_LENGTH	150
#define WINDOW_MENU_WIDTH	WINDOW_NUM_MENU * 16

/**Menu�Ĵ�����*/
void window_menu_handle(unsigned int y)
{
	window_print(GUI_control, "Clicked:%s", window_menu_lib[y / 16].text);
	run(window_menu_lib[y / 16].filename, NULL, 0);
}

/**����Menu����*/
void window_show_menu(void)
{
		/**���ø���ͼ��Ϊ��ʾ״̬*/
		attach_layer->visiable = true;
		
		/**���ø���ͼ���λ���������������Ϸ�*/
		GUI_set_position(attach_layer, 0, Video_Info.yres - (TASKBAR_WIDTH + WINDOW_MENU_WIDTH));
		
		/**���ø���ͼ��ĳ���*/
		GUI_set_resolution(attach_layer, WINDOW_MENU_LENGTH, WINDOW_MENU_WIDTH);
		
		/**������*/
		GUI_put_square(attach_layer, WINDOW_MENU_COLOR, 0, 1, WINDOW_MENU_LENGTH - 1, WINDOW_MENU_WIDTH - 2);
		
		/**���Ʊ߼���*/
		GUI_line(attach_layer, WINDOW_MENU_LINE_COLOR, 0, 0, WINDOW_MENU_LENGTH, 0);
		GUI_line(attach_layer, WINDOW_MENU_LINE_COLOR, WINDOW_MENU_LENGTH - 1, 1, WINDOW_MENU_LENGTH - 1, WINDOW_MENU_WIDTH - 2);
		
		unsigned long y_count;
		
		/**����ȫ�˵���Ϣ*/
		for (y_count = 0; y_count < WINDOW_NUM_MENU; y_count ++)
		{
			GUI_put_string(attach_layer, 0xff000000, 10, y_count * 16, 0, 0, font("Standard Font"), window_menu_lib[y_count].text);
		}
}

/**Application�Ƿ��ڹ����ı�־*/
bool Application_work_flag = false;

/**���Application�������*/
void window_click_Application(void)
{
/**�ж��Ƿ񱻵����*/

	/**����������*/
	if (Application_work_flag == true)
	{
		/**���Ϊδ�����*/
		Application_work_flag = false;
		
		/**��������״̬��Application*/
		window_draw_Application(TASKBAR_COLOR, 0xff000000);
		
		/**Application��������*/
		attach_layer->visiable = false;
		GUI_refresh_block(attach_layer->x, attach_layer->y, attach_layer->length, attach_layer->width);
		
	/**���δ�������*/
	}else
	{
		/**���Ϊ�Ѿ����*/
		Application_work_flag = true;
		
		/**���ƹ���״̬��Application*/
		window_draw_Application(0xff0000ff, 0xffffffff);
		
		/**��ʾ�˵�*/
		window_show_menu();
	}
}

/**����Application*/
void window_draw_Application(unsigned int backcolor, unsigned int fontcolor)
{
	/**����Application��*/
	GUI_put_square(taskbar_layer, backcolor, 0, 1, window_Application_length, 19);
	
	/**д��*/
	GUI_put_string(taskbar_layer, fontcolor, 4, 4, 0, 0, font("Standard Font"), "Application");
}
