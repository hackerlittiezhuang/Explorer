/*
 *Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.
 *made by √‘≤ ∫Ï–«<1@GhostBirdOS.org>
 *Explorer 0.01 main.cÕ∑
 *Explorer/Kernel/include/main.h
 *7/24/2014 10:56 AM
 */

#ifndef MAIN_H_
#define MAIN_H_

//void kernel_start(void);
void cmos_info(void);
void second(void);
struct CMOS
   {
      unsigned char current_second;
      unsigned char alarm_second;
      unsigned char current_minute;
      unsigned char alarm_minute;
      unsigned char current_hour;
      unsigned char alarm_hour;
      unsigned char current_day_of_week;
      unsigned char current_day;
      unsigned char current_month;
      unsigned char current_year;
      unsigned char status_registers[4];
      unsigned char diagnostic_status;
      unsigned char shutdown_code;
      unsigned char drive_types;
      unsigned char reserved_x;
      unsigned char disk_1_type;
      unsigned char reserved;
      unsigned char equipment;
      unsigned char lo_mem_base;
      unsigned char hi_mem_base;
      unsigned char hi_exp_base;
      unsigned char lo_exp_base;
      unsigned char fdisk_0_type;
      unsigned char fdisk_1_type;
      unsigned char reserved_2[19];
      unsigned char hi_check_sum;
      unsigned char lo_check_sum;
      unsigned char lo_actual_exp;
      unsigned char hi_actual_exp;
      unsigned char century;
      unsigned char information;
      unsigned char reserved3[12];
} cmos;
void main(void);
void test(void);
void load_COPR(void);
void load_BACKDROP_PIC(void);
void put_info(void);

#endif
