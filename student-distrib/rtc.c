#include "rtc.h"
#include "lib.h"


/*
 *  void rtc_init()
 *  DESC: initializes the rtc
 *  INPUTS: none
 *  OUTPUTS: none
 */
void rtc_init()
{
    // cli(); // clear interrupt
    // outb(RTC_REG_A, RTC_SELECT);
    // outb(0x20, 0x71); 
    set_rtc_freq(0x0F); 

    outb(RTC_REG_B, RTC_SELECT);    // select register B and disable NMI
    char prev = inb(RTC_CMOS);
    outb(RTC_REG_B, RTC_SELECT);
    outb(prev | BIT_6_MASK, RTC_CMOS);

    enable_irq(RTC_IRQ);
    enable_irq(RTC_IRQ2);
    // enable_irq(0);

    return;

}


/*
 * void rtc_handler()
 * DESC: handles interrupts for rtc
 * INPUTS: none
 * OUTPUTS: none
 */
void rtc_handler()
{
    // cli();
    test_interrupts();
    outb(RTC_REG_C, RTC_SELECT);
    inb(RTC_CMOS);
    rtc_interrupted = 1;
    send_eoi(RTC_IRQ);
    // sti();                  //interrupt process complete
    
    
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


    outb(RTC_REG_A, RTC_SELECT); // disable NMI and set index to reg a
    char prev = inb(RTC_CMOS);   // get current value of REG A
    outb(RTC_REG_A, RTC_SELECT);

    int32_t set_rate = (prev & 0xF0) | changed_rate;
    outb(set_rate, RTC_CMOS);
    return 1;
}

// reset freq to 2 Hz and return 0
int32_t rtc_open(const char* filename)
{
    set_rtc_freq(2);
    return 0;
}

// close rtc
int32_t rtc_close(int32_t fd)
{
    return 0;
}
