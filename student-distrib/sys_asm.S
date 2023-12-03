#define ASM     1

.data
jump_table:
    .long 0x0, sys_halt, sys_exec, sys_read, sys_write, sys_open, sys_close, sys_getargs, sys_vidmap

.text
.globl sys_call
.align 4
	
sys_call:

	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %esp
	pushl %ebp
	pushl %esi
	pushl %edi
	pushfl

	pushl %edx
	pushl %ecx
	pushl %ebx
	
    cmpl    $0x1, %eax
    jb      ERR
    cmpl    $0xA, %eax // make sure to change for when we add in the rest of the syscalls 
    ja      ERR
	
	call *jump_table(, %eax, 4);


done:
	// pop the arguments
	addl $12, %esp 

	// restore caller save reg
	popfl
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
	popl %ebx
	popl %edx
	popl %ecx

	iret

ERR:    // out of index sys_call
    movl    $-1, %eax
    jmp     done


halt_ret:
    pushl %ebp
    movl %esp, %ebp

    xorl %ebx, %ebx
    movl 16(%ebp), %ebx  // 3rd arg(status)
    movl %ebx, %eax         

    movl 12(%ebp), %ebx  // 2nd arg(esp)
    movl %ebx, %esp   

    movl 8(%ebp), %ebx   // 1sts arg(ebp)
    movl %ebx, %ebp

    leave
    ret

