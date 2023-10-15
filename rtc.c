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
    sti();
    send_eoi(RTC_IRQ);
}
