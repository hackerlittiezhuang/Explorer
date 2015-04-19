/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer IDE Disk Support Definition
 * Explorer/drivers/hdd.h
 * version:Alpha
 * 7/18/2014 5:48 PM
 */

#ifndef HDD_H_
#define HDD_H_

#include "types.h"

struct HD_info
{
	unsigned short wGenConfig;				// WORD 0: ������Ϣ��
	unsigned short wNumCyls;				// WORD 1: ������
	unsigned short wReserved2;				// WORD 2: ����
	unsigned short wNumHeads;				// WORD 3: ��ͷ��
	unsigned short wReserved4;				// WORD 4: ����
	unsigned short wReserved5;				// WORD 5: ����
	unsigned short wNumSectorsPerTrack;		// WORD 6: ÿ�ŵ�������
	unsigned short wVendorUnique[3];		// WORD 7-9: �����趨ֵ
	char sSerialNumber[20];					// WORD 10-19:���к�
	unsigned short  wBufferType;			// WORD 20: ��������
	unsigned short  wBufferSize;			// WORD 21: �����С
	unsigned short  wECCSize;				// WORD 22: ECCУ���С
	char sFirmwareRev[8];					// WORD 23-26: �̼��汾
	char sModelNumber[40];					// WORD 27-46: �ڲ��ͺ�
	unsigned short  wMoreVendorUnique;		// WORD 47: �����趨ֵ
	unsigned short  wReserved48;			// WORD 48: ����
	struct {
		unsigned short  reserved1:8;
		unsigned short  DMA:1;				// 1=֧��DMA
		unsigned short  LBA:1;				// 1=֧��LBA
		unsigned short  DisIORDY:1;			// 1=�ɲ�ʹ��IORDY
		unsigned short  IORDY:1;			// 1=֧��IORDY
		unsigned short  SoftReset:1;		// 1=��ҪATA������
		unsigned short  Overlap:1;			// 1=֧���ص�����
		unsigned short  Queue:1;			// 1=֧���������
		unsigned short  InlDMA:1;			// 1=֧�ֽ����ȡDMA
	}wCapabilities;							// WORD 49: һ������
	unsigned short  wReserved1;				// WORD 50: ����
	unsigned short  wPIOTiming;				// WORD 51: PIOʱ��
	unsigned short  wDMATiming;				// WORD 52: DMAʱ��
	struct {
		unsigned short  CHSNumber:1;		// 1=WORD 54-58��Ч
		unsigned short  CycleNumber:1;		// 1=WORD 64-70��Ч
		unsigned short  UnltraDMA:1;		// 1=WORD 88��Ч
		unsigned short  reserved:13;
	}wFieldValidity;						// WORD 53: �����ֶ���Ч�Ա�־
	unsigned short  wNumCurCyls;			// WORD 54: CHS��Ѱַ��������
	unsigned short  wNumCurHeads;			// WORD 55: CHS��Ѱַ�Ĵ�ͷ��
	unsigned short  wNumCurSectorsPerTrack;	// WORD 56: CHS��Ѱַÿ�ŵ�������
	unsigned short  wCurSectorsLow;			// WORD 57: CHS��Ѱַ����������λ��
	unsigned short  wCurSectorsHigh;		// WORD 58: CHS��Ѱַ����������λ��
	struct {
		unsigned short  CurNumber:8;		// ��ǰһ���Կɶ�д������
		unsigned short  Multi:1;			// 1=��ѡ���������д
		unsigned short  reserved1:7;
	}wMultSectorStuff;						// WORD 59: ��������д�趨
	unsigned long  dwTotalSectors;			// WORD 60-61: LBA��Ѱַ��������
	unsigned short  wSingleWordDMA;			// WORD 62: ���ֽ�DMA֧������
	struct {
		unsigned short Mode0:1;				// 1=֧��ģʽ0 (4.17Mb/s)
		unsigned short Mode1:1;				// 1=֧��ģʽ1 (13.3Mb/s)
		unsigned short Mode2:1;				// 1=֧��ģʽ2 (16.7Mb/s)
		unsigned short  Reserved1:5;
		unsigned short Mode0Sel:1;			// 1=��ѡ��ģʽ0
		unsigned short Mode1Sel:1;			// 1=��ѡ��ģʽ1
		unsigned short Mode2Sel:1;			// 1=��ѡ��ģʽ2
		unsigned short  Reserved2:5;
	}wMultiWordDMA;							// WORD 63: ���ֽ�DMA֧������
	struct {
		unsigned short  AdvPOIModes:8;		// ֧�ָ߼�POIģʽ��
		unsigned short  reserved:8;
	}wPIOCapacity;							// WORD 64: �߼�PIO֧������
	unsigned short wMinMultiWordDMACycle;	// WORD 65: ���ֽ�DMA�������ڵ���Сֵ
	unsigned short wRecMultiWordDMACycle;	// WORD 66: ���ֽ�DMA�������ڵĽ���ֵ
	unsigned short wMinPIONoFlowCycle;		// WORD 67: ��������ʱPIO�������ڵ���Сֵ
	unsigned short wMinPOIFlowCycle;		// WORD 68: ��������ʱPIO�������ڵ���Сֵ
	unsigned short wReserved69[11];			// WORD 69-79: ����
	struct {
		unsigned short Reserved1:1;
		unsigned short ATA1:1;				// 1=֧��ATA-1
		unsigned short ATA2:1;				// 1=֧��ATA-2
		unsigned short ATA3:1;				// 1=֧��ATA-3
		unsigned short ATA4:1;				// 1=֧��ATA/ATAPI-4
		unsigned short ATA5:1;				// 1=֧��ATA/ATAPI-5
		unsigned short ATA6:1;				// 1=֧��ATA/ATAPI-6
		unsigned short ATA7:1;				// 1=֧��ATA/ATAPI-7
		unsigned short ATA8:1;				// 1=֧��ATA/ATAPI-8
		unsigned short ATA9:1;				// 1=֧��ATA/ATAPI-9
		unsigned short ATA10:1;				// 1=֧��ATA/ATAPI-10
		unsigned short ATA11:1;				// 1=֧��ATA/ATAPI-11
		unsigned short ATA12:1;				// 1=֧��ATA/ATAPI-12
		unsigned short ATA13:1;				// 1=֧��ATA/ATAPI-13
		unsigned short ATA14:1;				// 1=֧��ATA/ATAPI-14
		unsigned short Reserved2:1;
	}wMajorVersion;							// WORD 80: ���汾
	unsigned short wMinorVersion;			// WORD 81: ���汾
	unsigned short wReserved82[6];			// WORD 82-87: ����
	struct {
		unsigned short Mode0:1;				// 1=֧��ģʽ0 (16.7Mb/s)
		unsigned short Mode1:1;				// 1=֧��ģʽ1 (25Mb/s)
		unsigned short Mode2:1;				// 1=֧��ģʽ2 (33Mb/s)
		unsigned short Mode3:1;				// 1=֧��ģʽ3 (44Mb/s)
		unsigned short Mode4:1;				// 1=֧��ģʽ4 (66Mb/s)
		unsigned short Mode5:1;				// 1=֧��ģʽ5 (100Mb/s)
		unsigned short Mode6:1;				// 1=֧��ģʽ6 (133Mb/s)
		unsigned short Mode7:1;				// 1=֧��ģʽ7 (166Mb/s) ???
		unsigned short Mode0Sel:1;			// 1=��ѡ��ģʽ0
		unsigned short Mode1Sel:1;			// 1=��ѡ��ģʽ1
		unsigned short Mode2Sel:1;			// 1=��ѡ��ģʽ2
		unsigned short Mode3Sel:1;			// 1=��ѡ��ģʽ3
		unsigned short Mode4Sel:1;			// 1=��ѡ��ģʽ4
		unsigned short Mode5Sel:1;			// 1=��ѡ��ģʽ5
		unsigned short Mode6Sel:1;			// 1=��ѡ��ģʽ6
		unsigned short Mode7Sel:1;			// 1=��ѡ��ģʽ7
	}wUltraDMA;								// WORD 88:  Ultra DMA֧������
	unsigned short wReserved89[167];		// WORD 89-255
};

struct Partition_Table{
	u8 active;
	char code[15];
};

struct Master_Boot_Record{
	char code[446];
	struct Partition_Table PT_1, PT_2, PT_3, PT_4;
	short int valid;
};

u32 LBA_start;
void inti_hdd(void);
void read_disk(u32 LBA, u16 *buffer, u32 number);
void hdd_wait(void);

/**���̲�����ȡ����*/
static void HD_get_phy_info(struct HD_info *info);

void int_HDC_handle(void);

#endif

