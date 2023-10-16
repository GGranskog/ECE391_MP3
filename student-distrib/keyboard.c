#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
// An array to map scan codes to corresponding characters (layout-based)

char keyboard[51] =
{
    NULL, 'e','1','2','3','4','5','6','7','8','9',
    '0','-','=','b','t','Q','W','E','R','T',
    'Y','U','I','O','P','[',']','e','l','A',
    'S','D','F','G','H','J','K','L',';','\'',
    '`','s','\\', 'Z', 'X', 'C', 'V','B', 'N', 'M'
};
// Keyboard interrupt handler

void keyboard_handler(){

    cli();// Disable interrupts to prevent interference during handling
    uint8_t code = inb(0x60);    // Read scan code from the keyboard controller (PS/2)
   
    // Check if the code is within a valid range

    if(code < 52){
        putc(keyboard[code]);        // Output the corresponding character to the screen

    }
send_eoi(1);    // Send an End of Interrupt (EOI) signal to the PIC controller for IRQ 1 (keyboard)

sti();// Re-enable interrupts
}
// Function to initialize the keyboard

void keyboard_init(){
    enable_irq(1);    // Enable IRQ 1 (keyboard interrupt) to allow keyboard input

}
