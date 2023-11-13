#define ASM     1

.data
jump_table:
    .long 0x0, sys_halt, sys_exec, sys_read, sys_write, sys_open, sys_close

.text
.globl sys_call

sys_call:
    // pushfl
	pushfl
	pushl %esi
	pushl %edi
	pushl %ebp
	
    cmpl    0x1, %eax
    jb      ERR
    cmpl    0xA, %eax // make sure to change for when we add in the rest of the syscalls 
    ja      ERR

	pushl %edx
	pushl %ecx
	pushl %ebx
	cli
	call *jump_table(, %eax, 4);

	# pop the arguments
	popl %ebx
	popl %ecx
	popl %edx

done:
	# restore caller save reg
	popl %ebp
	popl %edi
	popl %esi
	popfl

	iret

ERR:    // out of index sys_call
    movl    $-1, %eax
    jmp     done

