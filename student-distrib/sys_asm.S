#define ASM     1

.text
.globl sys_call

sys_call:
    pushal
    pushfl
    call rtc_handler
    popfl
    popal

    iret
