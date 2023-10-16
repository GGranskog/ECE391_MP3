#include "rtc.h"
#include "lib.h"
#include "i8259.h"

void rtc_init()
{
    int rate;

    cli(); // clear interrupt
    outb(RTC_REG_B, RTC_SELECT);    // select register B and disable NMI
    char prev = inb(RTC_CMOS);
    outb(RTC_REG_B, RTC_SELECT);
    outb(prev | BIT_6_MASK, RTC_CMOS);

    rate &= RTC_2; // set rate to 2 Hz
    outb(RTC_REG_A, RTC_SELECT);     // write to reg A
    prev = inb(RTC_CMOS);
    outb(RTC_REG_A, RTC_SELECT);
    outb((prev & 0xF0) | rate, RTC_CMOS);

    outb(RTC_REG_C, RTC_SELECT); // write to reg C
    inb(RTC_CMOS);

    enable_irq(RTC_IRQ);
    enable_irq(RTC_IRQ2);

    return;

}

void rtc_handler()
{
    cli();
    outb(RTC_REG_C, RTC_SELECT);
    inb(RTC_CMOS);
    rtc_interrupted = 1;
    sti();                  //interrupt process complete
    send_eoi(RTC_IRQ);
}

int32_t set_rtc_freq(int32_t freq)
{
    char changed_rate;

    switch(freq) {
        case 2:
            changed_rate = RTC_2;
            break;
        case 4:
            changed_rate = RTC_4;
            break;
        case 8:
            changed_rate = RTC_8;
            break;
        case 16:
            changed_rate = RTC_16;
            break;
        case 32:
            changed_rate = RTC_32;
            break;
        case 64:
            changed_rate = RTC_64;
            break;
        case 128:
            changed_rate = RTC_128;
            break;
        case 256:
            changed_rate = RTC_256;
            break;
        case 512:
            changed_rate = RTC_512;
            break;
        case 1024:
            changed_rate = RTC_1024;
            break;
        default:
            return -1;
            break;
    }

    cli();
    outb(RTC_REG_A, RTC_SELECT); // disable NMI and set index to reg a
    char prev = inb(RTC_CMOS);   // get current value of REG A
    outb(RTC_REG_A, RTC_SELECT);

    int32_t set_rate = (prev & 0xF0) | changed_rate;
    outb(set_rate, RTC_CMOS);
    sti();
    return 1;
}

// reset freq to 2 Hz and return 0
int32_t rtc_open(const char* filename)
{
    set_rtc_freq(2);
    return 0;
}

// close rtc
int32_t rtc_close(int32_t* inode)
{
    *inode = 0;
    return 0;
}

// read from rtc
int32_t rtc_read(int32_t* inode, uint32_t* offset, char* buf, uint32_t len)
{
    *offset = *inode;
    while(*offset > 0) wait();
    return 0;
}

int32_t rtc_write(int32_t* inode, uint32_t* offset, const char* buf, uint32_t len)
{
    // check if input is valid
    if(sizeof(uint32_t) != len) return -1;
    if(buf == NULL) return -1;

    // change rtc freq
    uint32_t freq = *(uint32_t*)buf;
    *inode = RTC_1024 / freq;
    return 0;
}

void wait()
{
    asm volatile("hlt");
}
