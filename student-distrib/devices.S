.text

.globl rtc_asm
.globl keyboard_asm

rtc_asm:
    pushal
    pushfl
    call rtc_handler
    popfl
    popal

    iret

keyboard_asm:
    pushal
    pushfl
    call keyboard_handler
    popfl
    popal

    iret
