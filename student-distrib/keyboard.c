#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

char keyboard[51] = 
{
    NULL, 'e','1','2','3','4','5','6','7','8','9',
    '0','-','=','b','t','Q','W','E','R','T',
    'Y','U','I','O','P','[',']','e','l','A',
    'S','D','F','G','H','J','K','L',';','\'',
    '`','s','\\', 'Z', 'X', 'C', 'V','B', 'N', 'M'
};

void keyboard_handler(){

    cli();
    uint8_t code = inb(0x60);
    if(code < 52){
        putc(keyboard[code]);
    }
send_eoi(1);
sti();
}

void keyboard_init(){
    enable_irq(1);
}