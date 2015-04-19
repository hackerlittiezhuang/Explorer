/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/ipc/msg.c
 * version:Alpha
 * 1/24/2015 2:27 AM
 */

/**消息机制实现*/

#include <lib/mem.h>
#include <memory.h>
#include <task.h>
#include <stddef.h>
#include <stdlib.h>

/**消息内容的最大长度*/
#define MAX_MSG		4096

/**消息结构体*/
struct msg
{
	struct msg *next;				/**指向下一个消息结构体*/
	task_id sender, object;			/**发送者和接收者*/
	void *message;					/**消息指针*/
	size_t size;					/**消息长度*/
};


/**发送消息函数*/
int send_msg(task_id object, void *msg, size_t size)
{
	/**分配新的区域存放消息*/
	if (size > MAX_MSG) error("msg error!");		/**判断消息长度是否超过限制*/
	void *temp_msg;
	temp_msg = kmalloc(size, 0);
	if (temp_msg == NULL) error("msg error!");
	
	/**分配新的消息结构*/
	struct msg *new_msg;
	new_msg = kmalloc(sizeof(struct msg), 0);
	if (new_msg == NULL) error("msg error!");
	
	/**拷贝消息*/
	memcpy(temp_msg, msg, size);
	
	/**填充消息结构*/
	new_msg->sender = current;
	new_msg->object = object;
	new_msg->message = temp_msg;
	new_msg->size = size;
	
	/**将该消息加入目标任务的消息队列上*/
	struct msg *list_point;
	for (list_point = object->info.msg_list; list_point->next != NULL; list_point = list_point->next);
	list_point->next = new_msg;
	new_msg->next = NULL;
	
	/**正常返回*/
	return 0;
}

/**接收消息函数
 * 该函数取出消息并从消息队列移除此消息
 * 返回值：1 - 成功；0 - 无消息；-1 - 消息过大
 */
int recv_msg(void *ptr, size_t msg_size)
{
	struct msg *msg_list = current->info.msg_list;
	
	/**检查是否存在消息*/
	if (msg_list == NULL) return 0;
	
	/**判断消息的长度是否超过接收消息的长度限制*/
	if (msg_list->size > msg_size) return -1;
	
	/**将消息拷贝出来*/
	memcpy(ptr, msg_list->message, msg_list->size);
	
	/**从当前任务中移除这个消息*/
	current->info.msg_list = msg_list->next;
	
	/**回收内存*/
	kfree(msg_list->message);
	kfree(msg_list);
	
	/**正常返回*/
	return 1;
}
