#define ASM     1
#include "sys_asm.h"

.text
.globl sys_call

sys_call:
    pushal
    pushfl
    call rtc_handler
    popfl
    popal

    iret