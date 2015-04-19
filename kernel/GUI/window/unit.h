/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window Unit Management Definition
 * Explorer/GUI/unit.h
 * version:Alpha
 * 2/19/2015
 */

#ifndef UNIT_H_
#define UNIT_H_

/**��Ԫ��Ϣ������ָ������*/
#define NUM_UNIT_HANDLE		3
extern void (*unit_handle[NUM_UNIT_HANDLE])(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**��Ԫ�����б�*/
#define UNIT_CLOSE			0
#define UNIT_MINI			1
#define UNIT_BOTTON			2

/**��Ԫ�ṹ*/
struct layer_unit
{
	unsigned int type;				/**��Ԫ������*/
	unsigned long x, y;				/**��Ԫ��λ��*/
	unsigned long length, width;	/**��Ԫ�ĳ���*/
	struct layer_unit *next;		/**��һ����Ԫ�ṹ*/
	void *ptr;						/**������Ԫ�Զ����ָ��*/
};

/**��ť��Ϣ������*/
void unit_botton_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**�½���ť����*/
struct layer_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width, char *text);

/**����رհ�ť������*/
void unit_close_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**������С����ť������*/
void unit_mini_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**�����µ�Ԫ����*/
struct layer_unit *GUI_new_unit(struct layer *target, unsigned int type, unsigned long x, unsigned long y, unsigned long length, unsigned long width);


#endif
