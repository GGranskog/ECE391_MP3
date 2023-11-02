#define ASM     1

.text
.globl sys_call

sys_call:
    pushl  %ebx
    movl    0x08(%esp), %ebx
    movl    $0x05, %eax
    int     $0x80
    cmpl    $0xFFFFF001,  %eax
    jb      done
    xorl    %eax, %edx
    subl    %eax, %edx
    pushl   %edx
    call    __errno_location
    popl    %ecx
    movl    %ecx, (%eax)
    orl     $0xFFFFFFFF, %eax

done:
    popl   %ebx
    ret

__errno_location:
    call    getIP
raddr:
    addl    $errno-raadr, %eax
    ret
getIP:
    movl    (%esp), %eax
    ret