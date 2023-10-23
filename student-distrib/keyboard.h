
/* keyboard.h - Defines used in interactions with the keyboard interrupt
 * controller
 */


#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

/* initialize keyboard by enabling irq 1 in pic */
void keyboard_init(void);



int check_for_modifier(uint8_t scan_code);

/* reads input from keyboard dataport when an interrupt occurs and put it to video memory */
extern void keyboard_handler(void);

void get_char(char new_char);

#endif /* KEYBOARD_H */

