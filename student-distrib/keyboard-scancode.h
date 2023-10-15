
#ifndef _KEYBOARD_SCANCODE_H_
#define _KEYBOARD_SCANCODE_H_

#define SCANCODE_TABLE_SIZE 58

char scan_to_ascii[SCANCODE_TABLE_SIZE][2] = {
   {   0,0   } ,
   {   0,0   } ,
   { '1','!' } ,
   { '2','@' } ,
   { '3','#' } ,
   { '4','$' } ,
   { '5','%' } ,
   { '6','^' } ,
   { '7','&' } ,
   { '8','*' } ,
   { '9','(' } ,
   { '0',')' } ,
   { '-','_' } ,
   { '=','+' } ,
   {   8,8   } ,
   {   0,0   } , 
   { 'q','Q' } ,
   { 'w','W' } ,
   { 'e','E' } ,
   { 'r','R' } ,
   { 't','T' } ,
   { 'y','Y' } ,
   { 'u','U' } ,
   { 'i','I' } ,
   { 'o','O' } ,
   { 'p','P' } ,
   { '[','{' } ,
   { ']','}' } ,
   {  10,10  } , 
   {   0,0   } ,
   { 'a','A' } ,
   { 's','S' } ,
   { 'd','D' } ,
   { 'f','F' } ,
   { 'g','G' } ,
   { 'h','H' } ,
   { 'j','J' } ,
   { 'k','K' } ,
   { 'l','L' } ,
   { ';',':' } ,
   {  39,34  } ,
   { '`','~' } ,
   {   0,0   } ,
   { '\\','|'} ,
   { 'z','Z' } ,
   { 'x','X' } ,
   { 'c','C' } ,
   { 'v','V' } ,
   { 'b','B' } ,
   { 'n','N' } ,
   { 'm','M' } ,
   { ',','<' } ,
   { '.','>' } ,
   { '/','?' } ,
   {   0,0   } ,
   {   0,0   } ,
   {   0,0   } ,
   { ' ',' ' } ,
};

/* keyboard irq number */
#define KEYBOARD_IRQ_NUM    1

#define KEYBOARD_DATA_PORT  (0x60)
#define KEYBOARD_CMD_PORT   (0x64)
#define SCANCODES_SIZE      (58)
#define RELEASE_OFFSET      (0x80)

#define BUFFER_MAX         (127)
#define BUFFER_SIZE        (128)


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

#endif