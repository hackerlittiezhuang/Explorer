/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface API
 * Explorer/GUI/gui_api.c
 * version:Alpha
 * 2/8/2015 11:41 PM
 */

/**API列表*/
#define NEW_WINDOW	1

/**新窗口API*/
struct new_window
{
	char *title;
	unsigned int style;
	unsigned int length;
	unsigned int width;
};

/**GUI API调用函数*/
int sys_GUI_API(unsigned int function, void *strptr)
{
	int retval = 0;
	
	/**分发功能*/
	switch(function)
	{
		case NEW_WINDOW: goto new_window;
		default: break;
	}
	
/**新窗口处理程序*/
new_window:
	retval = (int) GUI_Window(  ((struct new_window *)strptr)->title,
								((struct new_window *)strptr)->style,
								((struct new_window *)strptr)->length,
								((struct new_window *)strptr)->width);
	
	/**正常返回*/
	return retval;
}