#include "rtc.h"
#include "lib.h"

void rtc_init()
{
    // cli(); // clear interrupt
    outb(RTC_REG_A, RTC_SELECT);
    outb(0x20, 0x71);


    outb(RTC_REG_B, RTC_SELECT);    // select register B and disable NMI
    char prev = inb(RTC_CMOS);
    outb(RTC_REG_B, RTC_SELECT);
    outb(prev | BIT_6_MASK, RTC_CMOS);

    enable_irq(RTC_IRQ);
    enable_irq(RTC_IRQ2);
    // enable_irq(0);

    return;

}

void rtc_handler()
{
    // test_interrupts();
    cli();
    outb(RTC_REG_C, RTC_SELECT);
    inb(RTC_CMOS);
    rtc_interrupted = 1;
    send_eoi(RTC_IRQ);
    sti();                  //interrupt process complete
    
    
}
