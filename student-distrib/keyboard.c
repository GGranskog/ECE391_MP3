#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

/* Keyboard Constants */
#define KEYBOARD_IRQ_NUM    1
#define KEYBOARD_DATA_PORT  (0x60)
#define KEYBOARD_CMD_PORT   (0x64)
#define SCANCODES_SIZE      (58)
#define BUFFER_MAX          (127)
#define LEFT_SHIFT_PRESS    (0x2A)
#define LEFT_SHIFT_RELEASE  (0xAA)
#define RIGHT_SHIFT_PRESS   (0x36)
#define RIGHT_SHIFT_RELEASE (0xB6)
#define CTRL_PRESS          (0x1D)
#define CTRL_RELEASE        (0x9D)
#define CAPS_PRESS          (0x3A)
#define BACKSPACE           (0x08)
#define ENTER               (0x0A)
#define Q_UP_LIMIT          (0x10)
#define P_LOW_LIMIT         (0x19)
#define A_UP_LIMIT          (0x1E)
#define L_LOW_LIMIT         (0x26)
#define Z_UP_LIMIT          (0x2C)
#define M_LOW_LIMIT         (0x32)
#define BUFFER_SIZE         128




/* Global Flags */
uint8_t l_shift_flag  = 0;
uint8_t r_shift_flag  = 0;
uint8_t ctrl_flag     = 0;
uint8_t caps_flag     = 0;

/* Enter Flag and Character Buffer */
volatile static int enter_flag = 0;
static int char_count = 0;
static char char_buffer[BUFFER_SIZE];

/* Number of Characters Entered */
int num_char = 0;

/* Scan Code to ASCII Mapping */
char scan_to_ascii[SCANCODES_SIZE][2] = {
    {0x0, 0x0}, {0x0, 0x0},     // Nothing, Nothing
    {'1', '!'}, {'2', '@'},
    {'3', '#'}, {'4', '$'},
    {'5', '%'}, {'6', '^'},
    {'7', '&'}, {'8', '*'},
    {'9', '('}, {'0', ')'},
    {'-', '_'}, {'=', '+'},
    {BACKSPACE, BACKSPACE}, {' ', ' '},  // Backspace, Tab(treat as equivalent to SPACE)
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

/* Function Name: keyboard_init
Inputs: None (no explicit function parameters)
Return Value: None (return type is void)
Function: This function is responsible for initializing the keyboard. It enables IRQ 1 (Interrupt Request 1) 
in the Programmable Interrupt Controller (PIC), which allows the system to respond to keyboard interrupts. 
By enabling this interrupt, the keyboard becomes operational, and the system can handle keyboard input.*/
void keyboard_init(void) {
    // Enable IRQ for the keyboard
    enable_irq(KEYBOARD_IRQ_NUM);
}

/*Function Name: keyboard_handler
Inputs: None (no explicit function parameters)
Return Value: None (return type is void)
Function: This function is the keyboard interrupt handler. It's responsible for handling keyboard input and processing key presses, 
including special keys like Enter, Backspace, Shift, and Ctrl. The function manages flags and buffers related to keyboard input,
 displaying characters on the screen, and handling special keyboard combinations like Ctrl + L (clear screen). */

extern void keyboard_handler(void) {
    // Start critical section
    cli();

    uint8_t scan_code = inb(KEYBOARD_DATA_PORT); // Read from keyboard

    if (check_for_modifier(scan_code)) {
        // Modifier key detected, exit interrupt handler
        send_eoi(KEYBOARD_IRQ_NUM);
        sti();
        return;
    }

    if (scan_code >= SCANCODES_SIZE || scan_code <= 1) {
        // Invalid scan code, exit interrupt handler
        send_eoi(KEYBOARD_IRQ_NUM);
        sti();
        return;
    }

    if (scan_to_ascii[scan_code][0] == '\n') {
        // Handle Enter key
        num_char = 0;
        putc(scan_to_ascii[scan_code][0]);
        get_char(scan_to_ascii[scan_code][0]);
    } else if (ctrl_flag > 0 && scan_to_ascii[scan_code][0] == 'l') {
        // Handle Ctrl + L (Clear screen)
        clear();
        uint16_t position = screen_y * NUM_COLS + screen_x;
        outb(0x0F, 0x3D4);
        outb((uint8_t)(position & 0xFF), 0x3D5);
        outb(0x0E, 0x3D4);
        outb((uint8_t)((position >> 8) & 0xFF), 0x3D5);
    } else if (scan_to_ascii[scan_code][0] == BACKSPACE) {
        // Handle Backspace key
        if (num_char > 0) {
            putc(scan_to_ascii[scan_code][0]);
            //get_char(scan_to_ascii[scan_code][0]);
            --num_char;
        }
    } else if ((l_shift_flag || r_shift_flag) && num_char < BUFFER_MAX) {
        // Handle Shifted key with character limit
        if (caps_flag &&
            ((scan_code >= Q_UP_LIMIT && scan_code <= P_LOW_LIMIT) ||
             (scan_code >= A_UP_LIMIT && scan_code <= L_LOW_LIMIT) ||
             (scan_code >= Z_UP_LIMIT && scan_code <= M_LOW_LIMIT))) {
            putc(scan_to_ascii[scan_code][0]);
            get_char(scan_to_ascii[scan_code][0]);
            ++num_char;
        } else {
            putc(scan_to_ascii[scan_code][1]);
            get_char(scan_to_ascii[scan_code][1]);
            ++num_char;
        }
    } else if (caps_flag && num_char < BUFFER_MAX) {
        // Handle Caps Lock key with character limit
        if ((scan_code >= Q_UP_LIMIT && scan_code <= P_LOW_LIMIT) ||
            (scan_code >= A_UP_LIMIT && scan_code <= L_LOW_LIMIT) ||
            (scan_code >= Z_UP_LIMIT && scan_code <= M_LOW_LIMIT)) {
            putc(scan_to_ascii[scan_code][1]);
            get_char(scan_to_ascii[scan_code][1]);
            ++num_char;
        } else {
            putc(scan_to_ascii[scan_code][0]);
            get_char(scan_to_ascii[scan_code][0]);
            ++num_char;
        }
    } else if (num_char < BUFFER_MAX) {
        // Default character handling
        putc(scan_to_ascii[scan_code][0]);
        get_char(scan_to_ascii[scan_code][0]);
        ++num_char;
    }

    // End critical section
    send_eoi(KEYBOARD_IRQ_NUM);
    sti();
}

/* Function Name: check_for_modifier
Inputs:
uint8_t scan_code: An 8-bit unsigned integer representing the scan code sent from the keyboard.
Return Value:
int: An integer (1 if a modifier key is detected, 0 otherwise).
Function: This function checks the provided scan code to determine if it corresponds to a modifier key, 
such as Shift, Ctrl, or Caps Lock. If a modifier key is detected, it updates global flags accordingly 
(e.g., l_shift_flag, r_shift_flag, ctrl_flag, caps_flag). The function returns 1 if a modifier key is detected
 and 0 if the scan code is not a modifier key. Modifier keys affect how other keys are interpreted, 
 like shifting characters to uppercase or controlling special keyboard functions.*/

int check_for_modifier(uint8_t scan_code) {
    switch(scan_code) {
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
        case CTRL_PRESS:
            ctrl_flag += 1;
            return 1;
        case CTRL_RELEASE:
            ctrl_flag -= 1;
            return 1;
        case CAPS_PRESS:
            caps_flag = !caps_flag;
            return 1;
        default:
            return 0;  // Not a modifier key
    }
}

/* void get_char(char new_char);
 * Inputs: new_char - The character entered by the keyboard
 * Return Value: None
 * Function: Processes the keyboard input character and updates the character buffer and flags. */


void get_char(char new_char) {
    if (new_char == '\n') {
        // Set enter_flag and add newline character
        enter_flag = 1;
        char_buffer[char_count >= BUFFER_SIZE ? BUFFER_SIZE - 1 : char_count] = '\n';
    } else if (new_char == BACKSPACE) {
        // Handle Backspace
        if (char_count > 0) {
            putc(new_char);
            --char_count;
        }
    } else if (char_count < BUFFER_SIZE - 1) {
        // Add a new character to the buffer
        char_buffer[char_count] = new_char;
        ++char_count;
    } else {
        // Character buffer full
        ++char_count;
    }
}


/*

TERMINAL DRIVER READ/WRITE
*/



/* 
 *  terminal_read(int32_t fd, void* buf, int32_t nbytes)
 *  DESCRIPTION: just call keyboard_read
 *                 
 *  INPUTS: fd: file dir, not used for now
 *          const char* buf, the input buf
 *          int32_t length, length of the buf
 *  OUTPUTS: fill n Bytes in the buf
 *  RETURN VALUE: ret -- number of bytes read
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes) {
    int bytes_read = 0;
    int i;

    // Wait for Enter key press
    while (enter_flag == 0) {}

    cli();

    // Ensure nbytes is within buffer size
    nbytes = (nbytes < BUFFER_SIZE) ? nbytes : BUFFER_SIZE;

    for (i = 0; i < nbytes; ++i) {
        ((char*)buf)[i] = char_buffer[i];
        char_buffer[i] = ' ';

        // Exit if newline encountered
        if (((char*)buf)[i] == '\n') {
            bytes_read = i + 1;
            break;
        }

        // Ensure the last character in buf is newline
        if (i == nbytes - 1 && ((char*)buf)[i] != '\n') {
            ((char*)buf)[i] = '\n';
            bytes_read = i + 1;
            break;
        }
    }

    // If nbytes exceeds buffer size, continue filling the buffer
    if (nbytes >= BUFFER_SIZE) {
        for (i = nbytes; i < BUFFER_SIZE; ++i) {
            ((char*)buf)[i] = char_buffer[i];
            char_buffer[i] = ' ';

            // Exit if newline encountered
            if (((char*)buf)[i] == '\n') {
                bytes_read = i + 1;
                break;
            }
        }
    }

    char_count = 0;  // Reset char_buffer position
    enter_flag = 0;
    sti();

    return bytes_read;
}


/* 
 *  terminal_write(const char* buf, int32_t length)
 *  DESCRIPTION: write to the terminal with
 *              the contant in the buf of length
 *                 
 *  INPUTS: const char* buf, the input buf
 *          int32_t length, length of the buf
 *  OUTPUTS: write and display i=on the terminal
 *  RETURN VALUE: count -- number of bytes written 
 *                or -1 for fail
 */
int32_t terminal_write(int32_t fd, const void* input_buf, int32_t nbytes) {
    // Change buffer pointer type to char*
    const char* buf = input_buf;

    // Check if buf is invalid and return -1 in such cases
    if (buf == NULL) {
        return -1;
    }

    int32_t count = 0;  // Count the number of characters written to the screen

    // Write characters onto the screen
    int i;
    for (i=0; i < nbytes; i++) {
        putc(buf[i]);
        count++;
    }

    // Return the total number of bytes written
    return count;
}

/* 
 *  terminal_open(const uint8_t fname)
 *  DESCRIPTION: open the terminal
 *  INPUTS: const uint8_t* fname, the filename
 *  OUTPUTS: NONE
 *  RETURN VALUE: returns 0
 */
int32_t terminal_open (const uint8_t* filename){
int i;
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        char_buffer[i] = 0;
    }
    clear();
    return 0;
}

/* 
 *  terminal_close(int32_t fd)
 *  DESCRIPTION: closes the 
 *                 
 *  INPUTS: int32_t fd, the input file descriptor
 *  OUTPUTS: NONE
 *  RETURN VALUE: returns 0
 */
int32_t terminal_close (int32_t fd){
    return 0;
}

