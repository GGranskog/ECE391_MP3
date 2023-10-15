#ifndef _RTC_H
#define _RTC_H

#include "i8259.h"

// IRQ line connected to RTC
#define RTC_IRQ 8
#define RTC_IRQ2 2

// RTC register select and CMOS r/w ports
#define RTC_SELECT 0x70
#define RTC_CMOS 0x71
#define RTC_1024 0x06
#define RTC_512 0x07
#define RTC_256 0x08
#define RTC_128 0x09
#define RTC_64 0x0A
#define RTC_32 0x0B
#define RTC_16 0x0C
#define RTC_8 0x0D
#define RTC_4 0x0E
#define RTC_2 0x0F
#define BIT_6_MASK 0x40

// RTC register selector
#define RTC_REG_A 0x8A
#define RTC_REG_B 0x8B
#define RTC_REG_C 0x8C

volatile int rtc_interrupted;

extern void rtc_init();
extern void rtc_handler();

int32_t set_rtc_freq(int32_t freq)
int32_t rtc_open(int32_t* inode, char* filename);
int32_t rtc_close(int32_t* inode);
int32_t rtc_read(int32_t* inode, uint32_t* offset, char* buf, uint32_t len);
int32_t rtc_write(int32_t* inode, uint32_t* offset, const char* buf, uint32_t len)

#endif
