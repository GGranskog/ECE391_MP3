#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

char keyboard[51] = 
{
  '\0','\0', '1','2','3','4', '5', '6', '7','8','9','0,'-','=','\0', '\0',
  'q','w','e','r','t','y','u','i','o','p','[',']','\0', '\0',
  'a','s','d','f','g','h','j','k','l',';','\'','`','\0','\\',
  'z','x','c,'v','b','n','m'
};

void keyboard_handler(){

    cli();
    
    uint8_t scanline;
    scanline = inb(0x60);
    if(scanline < 52){
        putc(keyboard[scanline]);
    }
send_eoi(1);
sti();
}

void keyboard_init(){
    enable_irq(1);
}