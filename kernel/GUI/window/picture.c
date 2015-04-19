/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Picture
 * Explorer/GUI/window/picture.c
 * version:Alpha
 * 2/16/2015 7:55 PM
 */

#include <GUI.h>
#include <time.h>
#include <studio.h>
#include <stdlib.h>
#include <cfs.h>
#include <memory.h>
#include <lib/graphics.h>
#include "window.h"
#include "../layer.h"

#pragma pack(push)					//���浱ǰ������Ϣ
#pragma pack(1)						//�趨�ṹ����һ���ֽ�Ϊ��λ����

/**BMPλͼ�ļ�ͷ�ṹ*/
#define NUM_OF_TYPE		2
struct bmp_file_header
{
	char type[NUM_OF_TYPE];			//λͼ�����Windows�У�����ֶ�Ϊ"BM"
	unsigned int size;				//����
	unsigned int reserved;			//��������Ϊ0
	unsigned int bmp_data_offset;	//���ļ���ʼ��λͼ���ݿ�ʼ֮�������(bitmap data)֮���ƫ����
	unsigned int bmp_header_size;	//λͼ��Ϣͷ(bitmap info header)�ĳ���
	unsigned int length;			//λͼ����
	unsigned int width;				//λͼ���
	unsigned short planes;			//λͼ��λ����(����1)
};

/**BMPͼ����Ϣͷ
 * ѹ��˵��
 * 0 - ��ѹ�� (ʹ��BI_RGB��ʾ)
 * 1 - RLE 8-ʹ��8λRLEѹ����ʽ(��BI_RLE8��ʾ)
 * 2 - RLE 4-ʹ��4λRLEѹ����ʽ(��BI_RLE4��ʾ)
 * 3 - Bitfields-λ���ŷ�ʽ(��BI_BITFIELDS��ʾ)
 */
struct bmp_info_header
{
	unsigned short bpp;				//ÿ������λ��(1/4/8/16/24/32�ֱ����ɫ/16ɫ/256ɫ/16bit/24bit/32bit)
	unsigned int compression;		//ѹ��˵��
	unsigned int bmp_data_size;		//�ֽڱ�ʾ��λͼ���ݴ�С(4�ı���)
	unsigned int HResolution;		//������/�ױ�ʾˮƽ�ֱ���
	unsigned int VResolution;		//������/�ױ�ʾ��ֱ�ֱ���
	unsigned int colors;			//λͼʹ�õ���ɫ��
	unsigned int impt_colors;		//ָ����Ҫ����ɫ��
};

#pragma pack(pop)					//�ָ�ԭ���Ķ��뵥λ

/**
 * BMP��ʽͼƬ���غ���
 * �ɹ�����0�����ɹ�����1
 */
int *window_load_bmp(struct GUI_image *image, char *buffer)
{	
	/**дָ��Ͷ�ָ��*/
	unsigned long wptr, rptr;
	
	unsigned char *data;
	
	/**BMP������ָ��*/
	unsigned char *bmp_data;
	
	/**BMPλͼ�ļ�ͷָ��*/
	struct bmp_file_header *head = (struct bmp_file_header *) buffer;
	
	/**BMPλͼ��Ϣͷָ��*/
	struct bmp_info_header *info_head = (struct bmp_info_header *) (buffer + sizeof(struct bmp_file_header));
	
	/**�����Ϣ*/
	// window_print(GUI_control, "BMP size:%dByte,length:%d,width:%d,bpp:%d.", head->size, head->length, head->width, info_head->bpp);
	
	/**��ȡ��������ʼ��ַ*/
	bmp_data = buffer + head->bmp_data_offset;
	
	/**׼�����������ó����������*/
	for (data = NULL; data == NULL; )
		data = vmalloc(head->length * head->width * 4);
	
	/**����ÿ������λ��ʹ�ò�ͬ��������*/
	switch(info_head->bpp)
	{
		case 24: goto copy_24;
		case 32: goto copy_32;
	}
	
/**24λλͼ��������*/
copy_24:
	/**ѭ������*/
	wptr = 0;
	rptr = (head->width - 1) * head->length * 3;
	for (; wptr < head->length * head->width * 4;)
	{
		data[wptr + 0] = bmp_data[rptr + 0];
		data[wptr + 1] = bmp_data[rptr + 1];
		data[wptr + 2] = bmp_data[rptr + 2];
		data[wptr + 3] = 0xff;
		wptr += 4;
		rptr += 3;
		
		/**�������*/
		if (rptr % (head->length * 3) == 0)
		{
			rptr -= head->length * 3 * 2;
		}
	}
	goto finish;
	
/**32λλͼ��������*/
copy_32:	
	/**ѭ������*/
	wptr = 0;
	rptr = (head->width - 1) * head->length;
	for (; wptr < head->length * head->width;)
	{
		((unsigned int *)data)[wptr] = ((unsigned int *)bmp_data)[rptr];
		wptr ++;
		rptr ++;
		
		/**�������*/
		if (rptr % (head->length) == 0)
		{
			rptr -= head->length * 2;
		}
	}
	goto finish;
	
finish:
	/**���*image*/
	image->data = (unsigned int *)data;
	image->length = head->length;
	image->width = head->width;
	
	/**��������*/
	return 0;
}

/**����֧�ֵ��ļ���ͷ�ṹ������*/
union image_head_union
{
	struct bmp_file_header bmp_file_header;
};

/**����ͼƬ����*/
struct GUI_image *window_load_image(char *filename)
{
	/**�ļ���Ϣ�ṹ��*/
	file_info image_info;
	
	/**�ļ�������ָ��*/
	char *buffer;
	
	/**�ļ�ͷ�ṹ������*/
	union image_head_union *head_union;
	
	/**���صĳ���ͼ�νṹָ��*/
	struct GUI_image *retval;
	
	/**��ȡ�ļ���Ϣ*/
	image_info = get_file_info(filename);
	
	/**������ϴ�С�Ļ�����*/
	for (buffer = NULL; buffer == NULL; )
		buffer = vmalloc(image_info.size);
	
	/**������ϴ�С�ĳ���ͼ�νṹʹ�õ��ڴ�*/
	for (retval = NULL; retval == NULL; )
		retval = kmalloc(sizeof(struct GUI_image), 0);
	
	
	/**����ͼƬ�ļ�*/
	file_open(filename, buffer);
	
	/**ͷ�ṹ��ָ��ָ���ļ�ͷ��*/
	head_union = (union image_head_union *) buffer;
	
/**�ж�ͼƬ��ʽ���ֱ�ʹ�ò�ͬ��ת������ת��*/
	
	/**�ж��Ƿ���BMPͼƬ*/
	if (head_union->bmp_file_header.type[0] == 'B' &
		head_union->bmp_file_header.type[1] == 'M' )
	{
		/**��BMPͼƬ*/
		// window_print(GUI_control, "image type:Microsoft Windows BMP Image");
		
		/**����BMPͼƬ���غ���*/
		window_load_bmp(retval, buffer);
		
		/**�ͷŻ�����*/
		vfree(buffer);
		// window_print(GUI_control, "GUI image length:%d,width:%d,data:%#X", retval->length, retval->width, retval->data);
	}else{
		/**������ʽ�ļ�*/
		// window_print(GUI_control, "Unkown Format.");
		
		/**�ͷ���ؽṹռ�õ��ڴ桢�ļ�ռ�õĻ���*/
		vfree(buffer);
		kfree(retval);
		
		/**ʧ�ܷ���*/
		return NULL;
	}
	
	/**��������*/
	return retval;
}

/**�ͷ�ͼƬ��Դ����*/
void free_image(struct GUI_image *image)
{
	/**�ж��Ƿ���Ҫ�ͷ�*/
	if (image->data == NULL) return;
	
	/**�ͷ�image��������*/
	vfree(image->data);
	
	/**�ͷ�image�Ľṹ��*/
	kfree(image);
	
	/**�ÿ�ָ��*/
	image = NULL;
	
	/**��������*/
}
