#define ASM     1

.data
jump_table:
    .long 0x0, sys_execute, sys_halt, sys_read, sys_write, sys_open, sys_close

.text
.globl sys_call

sys_call:
    // pushfl
    pushl   %ebp    
    pushl   %edi
    pushl   %esi
    pushl   %edx    
    pushl   %ecx
    pushl   %ebx  

    cmpl    0x1, %eax
    jb      ERR
    cmpl    0x6, %eax // make sure to change for when we add in the rest of the syscalls 
    ja      ERR

    call    *jump_table(0 ,%eax, 4)

done:
    popl    %ebx
    popl    %ecx
    popl    %edx
    popl    %esi
    popl    %edi
    popl    %ebp
    // popfl
    iret

ERR:    // out of index sys_call
    movl    $-1, %eax
    jmp     done

