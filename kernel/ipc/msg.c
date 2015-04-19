/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/ipc/msg.c
 * version:Alpha
 * 1/24/2015 2:27 AM
 */

/**��Ϣ����ʵ��*/

#include <lib/mem.h>
#include <memory.h>
#include <task.h>
#include <stddef.h>
#include <stdlib.h>

/**��Ϣ���ݵ���󳤶�*/
#define MAX_MSG		4096

/**��Ϣ�ṹ��*/
struct msg
{
	struct msg *next;				/**ָ����һ����Ϣ�ṹ��*/
	task_id sender, object;			/**�����ߺͽ�����*/
	void *message;					/**��Ϣָ��*/
	size_t size;					/**��Ϣ����*/
};


/**������Ϣ����*/
int send_msg(task_id object, void *msg, size_t size)
{
	/**�����µ���������Ϣ*/
	if (size > MAX_MSG) error("msg error!");		/**�ж���Ϣ�����Ƿ񳬹�����*/
	void *temp_msg;
	temp_msg = kmalloc(size, 0);
	if (temp_msg == NULL) error("msg error!");
	
	/**�����µ���Ϣ�ṹ*/
	struct msg *new_msg;
	new_msg = kmalloc(sizeof(struct msg), 0);
	if (new_msg == NULL) error("msg error!");
	
	/**������Ϣ*/
	memcpy(temp_msg, msg, size);
	
	/**�����Ϣ�ṹ*/
	new_msg->sender = current;
	new_msg->object = object;
	new_msg->message = temp_msg;
	new_msg->size = size;
	
	/**������Ϣ����Ŀ���������Ϣ������*/
	struct msg *list_point;
	for (list_point = object->info.msg_list; list_point->next != NULL; list_point = list_point->next);
	list_point->next = new_msg;
	new_msg->next = NULL;
	
	/**��������*/
	return 0;
}

/**������Ϣ����
 * �ú���ȡ����Ϣ������Ϣ�����Ƴ�����Ϣ
 * ����ֵ��1 - �ɹ���0 - ����Ϣ��-1 - ��Ϣ����
 */
int recv_msg(void *ptr, size_t msg_size)
{
	struct msg *msg_list = current->info.msg_list;
	
	/**����Ƿ������Ϣ*/
	if (msg_list == NULL) return 0;
	
	/**�ж���Ϣ�ĳ����Ƿ񳬹�������Ϣ�ĳ�������*/
	if (msg_list->size > msg_size) return -1;
	
	/**����Ϣ��������*/
	memcpy(ptr, msg_list->message, msg_list->size);
	
	/**�ӵ�ǰ�������Ƴ������Ϣ*/
	current->info.msg_list = msg_list->next;
	
	/**�����ڴ�*/
	kfree(msg_list->message);
	kfree(msg_list);
	
	/**��������*/
	return 1;
}
