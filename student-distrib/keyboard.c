#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"

/* keyboard irq number */
#define KEYBOARD_IRQ_NUM    1

#define KEYBOARD_DATA_PORT  (0x60)
#define KEYBOARD_CMD_PORT   (0x64)
#define SCANCODES_SIZE      (58)
#define RELEASE_OFFSET      (0x80)

#define BUFFER_MAX         (127)

#define LEFT_SHIFT_PRESS    (0x2A)
#define LEFT_SHIFT_RELEASE  (0xAA)
#define RIGHT_SHIFT_PRESS   (0x36)
#define RIGHT_SHIFT_RELEASE (0xB6)
#define CTRL_PRESS          (0x1D)
#define CTRL_RELEASE        (0x9D)
#define CAPS_PRESS          (0x3A)

#define BCKSPACE            (0x08)
#define ENTER               (0x0A)
//Regions containing letter characters
#define Q_UP_LIMIT          (0x10)
#define P_LOW_LIMIT         (0x19)
#define A_UP_LIMIT          (0x1E)
#define L_LOW_LIMIT         (0x26)
#define Z_UP_LIMIT          (0x2C)
#define M_LOW_LIMIT         (0x32)

// Flags for modifier keys
uint8_t l_shift_flag  = 0;
uint8_t r_shift_flag  = 0;
uint8_t ctrl_flag   = 0;    //Can be up to two (for the right and left)
uint8_t caps_flag   = 0;

// Count to keep track of the available number of backspaces
int num_char    = 0;

// Check for modifiers and return 1 if a modifier was pressed
uint8_t check_for_modifier(uint8_t scan_code);

/*
* Store every key on the keyboard and its alternate function in a lookup table
* This was generated by pressing every key and then pressing shift + the key
* This took forever to type
*/
char scan_to_ascii[SCANCODES_SIZE][2] = {
    {0x0, 0x0}, {0x0, 0x0},     // Nothing, Nothing
    {'1', '!'}, {'2', '@'},
    {'3', '#'}, {'4', '$'},
    {'5', '%'}, {'6', '^'},
    {'7', '&'}, {'8', '*'},
    {'9', '('}, {'0', ')'},
    {'-', '_'}, {'=', '+'},
    {BCKSPACE, BCKSPACE}, {' ', ' '},     // Backspace, Tab(treat as equivalent to SPACE)
    {'q', 'Q'}, {'w', 'W'},
    {'e', 'E'}, {'r', 'R'},
    {'t', 'T'}, {'y', 'Y'},
    {'u', 'U'}, {'i', 'I'},
    {'o', 'O'}, {'p', 'P'},
    {'[', '{'}, {']', '}'},
    {ENTER, ENTER}, {0x0, 0x0},     // Enter, Left Control
    {'a', 'A'}, {'s', 'S'},
    {'d', 'D'}, {'f', 'F'},
    {'g', 'G'}, {'h', 'H'},
    {'j', 'J'}, {'k', 'K'},
    {'l', 'L'}, {';', ':'},
    {'\'', '"'}, {'`', '~'},
    {0x0, 0x0}, {'\\', '|'},    // Left Shift
    {'z', 'Z'}, {'x', 'X'},
    {'c', 'C'}, {'v', 'V'},
    {'b', 'B'}, {'n', 'N'},
    {'m', 'M'}, {',', '<'},
    {'.', '>'}, {'/', '?'},
    {0x0, 0x0}, {0x0, 0x0},     // Right Shift, Nothing
    {0x0, 0x0}, {' ', ' '},     // Nothing

};


/* void keyboard_init(void);
 * Inputs: void
 * Return Value: none
 * Function: initialize keyboard by enabling irq 1 in pic */
void keyboard_init(void) {
    enable_irq(KEYBOARD_IRQ_NUM);
}

/* extern void keyboard_handler(void);
 * Inputs: void
 * Return Value: none
 * Function: reads input from keyboard dataport when an interrupt occurs and put it to video memory
 *           and terminal_read's buffer.
 */
extern void keyboard_handler(void) {
    // Start critical section
    cli();

    uint8_t scan_code = inb(KEYBOARD_DATA_PORT);    // take input from keyboard

    if(check_for_modifier(scan_code)) {
        send_eoi(KEYBOARD_IRQ_NUM);
        sti();
        return;
    }

    if((scan_code < SCANCODES_SIZE) && (scan_code > 1)) {                // ignore released output from keyboard
        //Outputs new line and clears the backspace counter.
        if(scan_to_ascii[scan_code][0] == '\n'){
            num_char = 0;
            putc(scan_to_ascii[scan_code][0]);
        //Ctrl-l for clearing the screen
        } else if((ctrl_flag > 0) && (scan_to_ascii[scan_code][0] == 'l')){
            clear();
        //Deterimes if there are still enough characters for a backspace.
        } else if(scan_to_ascii[scan_code][0] == BCKSPACE){
            if(num_char > 0){
                putc(scan_to_ascii[scan_code][0]);
                --num_char;
            }
        //Outputs the shifted versions of the keys pressed.
    } else if((l_shift_flag || r_shift_flag) && (num_char < BUFFER_MAX)) {
            //If CAPS is active the letters are made to be lower case
            if(caps_flag &&
               (((scan_code >= Q_UP_LIMIT) && (scan_code <= P_LOW_LIMIT)) ||
               ((scan_code >= A_UP_LIMIT) && (scan_code <= L_LOW_LIMIT)) ||
               ((scan_code >= Z_UP_LIMIT) && (scan_code <= M_LOW_LIMIT))) ){
                putc(scan_to_ascii[scan_code][0]);
                ++num_char;
            }
            //Otherwise the shifted versions are outputted
            else{
                putc(scan_to_ascii[scan_code][1]);
                ++num_char;
            }
        //Prints out the captialized version if necessary to the screen
    } else if(caps_flag && (num_char < BUFFER_MAX)){
            if(((scan_code >= Q_UP_LIMIT) && (scan_code <= P_LOW_LIMIT)) ||
               ((scan_code >= A_UP_LIMIT) && (scan_code <= L_LOW_LIMIT)) ||
               ((scan_code >= Z_UP_LIMIT) && (scan_code <= M_LOW_LIMIT))){
                putc(scan_to_ascii[scan_code][1]);
                ++num_char;
            }
            //If it is not a letter it is printed as is.
            else{
                putc(scan_to_ascii[scan_code][0]);
                ++num_char;
            }
        } else if(num_char < BUFFER_MAX){
            putc(scan_to_ascii[scan_code][0]);
            ++num_char;
        }
    }

    send_eoi(KEYBOARD_IRQ_NUM);
    sti();
}

/* uint8_t check_for_modifier(uint8_t scan_code)
 * Inputs: scan_code - Index of scan code sent from keyboard
 * Return Value: 1 if modifier found, 0 otherwise
 * Function: Check if scan_code is a modifier key and update global flags */
uint8_t check_for_modifier(uint8_t scan_code) {
    switch(scan_code)
    {
    //For both shift cases (uses different codes)
    case LEFT_SHIFT_PRESS:
        l_shift_flag = 1;
        return 1;
    case LEFT_SHIFT_RELEASE:
        l_shift_flag = 0;
        return 1;
    case RIGHT_SHIFT_PRESS:
        r_shift_flag = 1;
        return 1;
    case RIGHT_SHIFT_RELEASE:
        r_shift_flag = 0;
        return 1;
    //Accounts for both right and left (uses same scan code)
    case CTRL_PRESS:
        ctrl_flag += 1;
        return 1;
    case CTRL_RELEASE:
        ctrl_flag -= 1;
        return 1;
    //For when the caps key is on or off.
    case CAPS_PRESS:
        if(caps_flag == 1)
          caps_flag = 0;
        else
          caps_flag = 1;
        return 1;
    default:
        return 0;
    }
}
