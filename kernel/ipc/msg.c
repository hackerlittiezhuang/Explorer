/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/ipc/msg.c
 * version:Alpha
 * 1/24/2015 2:27 AM
 */

/**
 * ��Ϣ����ʵ�� 2015.
 * Explorer��Ϣ�������ڽ��̼�ͨ�ŷ���֮һ
 * ������Ϣ�Ľ�����Ҫ�ṩ��Ϣtype���������
 * ��һ��32λ�޷�������������˵����Ϣ�����
 * ����0x00000000~0x7fffffff����ϵͳ������Ϣ��
 *     0x80000000~0xffffffff������ͨ���̷�����Ϣ��
 */

#include <lib/mem.h>
#include <syscalls.h>
#include <memory.h>
#include <task.h>
#include <stddef.h>
#include <stdlib.h>

/**��Ϣ���ݵ���󳤶�*/
#define MAX_MSG		4096

/**��Ϣ�ṹ��*/
struct msg
{
	struct msg *next;								// ָ����һ����Ϣ�ṹ��
	struct process_struct *sender, *object;			// �����ߺͽ�����
	void *message;									// ��Ϣָ��
	size_t size;									// ��Ϣ����
	unsigned int type;								// ��Ϣ�����
};


/**�ں˼�������Ϣ������type�����0x00000000~0xffffffff��*/
int send_msg(struct process_struct *object, unsigned int type, void *msg, size_t size)
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
	new_msg->sender = current->info.pptr;
	new_msg->object = object;
	new_msg->message = temp_msg;
	new_msg->size = size;
	new_msg->type = type;
	
	/**������Ϣ����Ŀ���������Ϣ������*/
	struct msg *list_point;
	for (list_point = object->msg_list; list_point->next != NULL; list_point = list_point->next);
	list_point->next = new_msg;
	new_msg->next = NULL;
	
	/**��������*/
	return 0;
}

/**��Ϣϵͳ�����ӹ��ܶ���*/
#define MSG_SEND	1

/**�û�����Ϣ���ͺ�����ϵͳ���ã�*/
int sys_msg(struct context context)
{	
/**���ܵ����ж�*/
	switch(context.ebx)
	{
		case MSG_SEND	: goto send_msg;
		default			: goto finish;
	}

/**
 * ������Ϣ����
 * ebx = MSG_SEND, ecx = size of msg, edx = ptr of msg, esi = type, edi = object
 * ����ֵ�����eax�У�Ϊ����״̬
 */
send_msg:
	/**�Բ��������ж�*/
	if (context.esi >= 0x80000000) return -1;
	
	/**������Ϣ���ͺ���*/
	context.eax = send_msg((struct process_struct *)context.edi, context.esi, (void *)context.edx, context.ecx);
	
/**��ɴ���*/
finish:
	/**��������*/
	return;
}

/**
 * ������Ϣ����
 * �ú���ȡ����Ϣ������Ϣ�����Ƴ�����Ϣ
 * ����ֵ��1 - �ɹ���0 - ����Ϣ��-1 - ��Ϣ����
 */
int recv_msg(void *ptr, size_t msg_size)
{
	struct msg *msg_list = current->info.pptr->msg_list;
	
	/**����Ƿ������Ϣ*/
	if (msg_list == NULL) return 0;
	
	/**�ж���Ϣ�ĳ����Ƿ񳬹�������Ϣ�ĳ�������*/
	if (msg_list->size > msg_size) return -1;
	
	/**����Ϣ��������*/
	memcpy(ptr, msg_list->message, msg_list->size);
	
	/**�ӵ�ǰ�������Ƴ������Ϣ*/
	current->info.pptr->msg_list = msg_list->next;
	
	/**�����ڴ�*/
	kfree(msg_list->message);
	kfree(msg_list);
	
	/**��������*/
	return 1;
}
