#include "rtc.h"
#include "lib.h"
#include "tests.h"


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
    set_rtc_freq(4); 

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
    //test_interrupts();
    outb(RTC_REG_C, RTC_SELECT);
    inb(RTC_CMOS);
    rtc_interrupted = 1;
    send_eoi(RTC_IRQ);
    // sti();                  //interrupt process complete
    
    
}

/*
 *  int32_t set_rtc_freq(int32_t freq)
 *  DESCRIPTION: Changes the clock frequency to freq
 *  INPUTS: new frequency freq, should be a power of 2
 *  OUTPUTS: 1 on success, -1 on fail
 *  SIDE EFFECTS: none
 */
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

/*
 *  int32_t rtc_open(const uint8_t* filename)
 *  DESCRIPTION: sets rtc freq to 2 Hz and returns 0
 *  INPUTS: filename - "string" for file to open
 *  OUTPUTS: always 0
 *  SIDE EFFECTS: none
 */
int32_t rtc_open(const uint8_t* filename)
{
    set_rtc_freq(2);
    return 0;
}

/*
 *  int32_t rtc_close(int32_t fd)
 *  DESCRIPTION: closes file and returns 0
 *  INPUTS: fd - 32 bit file descriptor
 *  OUTPUTS: always 0
 *  SIDE EFFECTS: none
 */
int32_t rtc_close(int32_t fd)
{
    return 0;
}

/*
 *  int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes)
 *  DESCRIPTION: reads from fd and returns 0
 *  INPUTS: fd - 32 bit file descriptor
 *          buf - buffer
 *          nbytes - number of bytes to read
 *  OUTPUTS: number of bytes reads
 *  SIDE EFFECTS: idles until rtc_interrupted is changed
 */

int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes)
{
    rtc_interrupted = 0;
    while(!rtc_interrupted) rtc_interrupted = 0;    //idle until handler is executed again

    return 0;       // return 0 once interrupt has occurred
}


/*
 *  int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes)
 *  DESCRIPTION: writes to fd
 *  INPUTS: fd - 32 bit file descriptor
 *          buf - buffer array
 *          nbytes - length of write
 *  OUTPUTS: returns fd if successful, -1 if not
 *  SIDE EFFECTS: changes rtc frequency
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes)
{
    if(set_rtc_freq(*(uint8_t*)(buf)))
    {
        return fd;
    }
    return -1;
}

