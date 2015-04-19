/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface API
 * Explorer/GUI/gui_api.c
 * version:Alpha
 * 2/8/2015 11:41 PM
 */

/**API�б�*/
#define NEW_WINDOW	1

/**�´���API*/
struct new_window
{
	char *title;
	unsigned int style;
	unsigned int length;
	unsigned int width;
};

/**GUI API���ú���*/
int sys_GUI_API(unsigned int function, void *strptr)
{
	int retval = 0;
	
	/**�ַ�����*/
	switch(function)
	{
		case NEW_WINDOW: goto new_window;
		default: break;
	}
	
/**�´��ڴ������*/
new_window:
	retval = (int) GUI_Window(  ((struct new_window *)strptr)->title,
								((struct new_window *)strptr)->style,
								((struct new_window *)strptr)->length,
								((struct new_window *)strptr)->width);
	
	/**��������*/
	return retval;
}