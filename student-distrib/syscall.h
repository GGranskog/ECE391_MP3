#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "tests.h"
#include "idt.h"
#include "types.h"
#include "paging.h"
#include "filesys.h"
#include "keyboard.h"
#include "rtc.h"

#define TASK_SIZE   0x2000  // 8kB  PCB size
#define PAGE_SIZE   0x0400000 // 4MB page size
#define KER_ADDR    0x0800000  // kernel page starts addr 8MB

#define MAG_EXEC_0  0x7f
#define MAG_EXEC_1  0x45
#define MAG_EXEC_2  0x4c
#define MAG_EXEC_3  0x46

#define vir_startaddr  0x08000000 // 128MB
#define vidmap_startaddr  0x08400000 // 132MB

#define VIRT_ADDR VIRTUAL //virtual address

typedef struct fop_table{
    int32_t (*sys_open)(const uint8_t* filename);
    int32_t (*sys_close)(int32_t fd);
    int32_t (*sys_read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*sys_write)(int32_t fd, const void* buf, int32_t nbytes);
}fop_table_t;

typedef struct fd_table{
    fop_table_t* fop_table_ptr;
    int32_t inode;
    int32_t file_pos;
    int32_t flags;
}fd_table_t;

typedef struct pcb{
    fd_table_t fda[8];      // file descriptor array
    uint8_t cmd[128];       // command
    uint8_t arg[128];       // arguments for the command
    uint32_t esp0;
    uint16_t ss0;
    uint32_t ebp;
    uint32_t esp;
    uint32_t task_ebp;
    uint32_t task_esp;
    // uint32_t ss;
    uint32_t eip;
    uint32_t pid;           // current process id
    uint32_t parent_pid;// parent process id
    // uint32_t eflags;
    // uint32_t cs;
    // uint32_t saved_file;

    // uint32_t sig;

}pcb_t;

fop_table_t rtc_fop;
fop_table_t dir_fop;
fop_table_t file_fop;
fop_table_t terminal_fop;
fop_table_t null_fop;

void fop_init();
pcb_t* get_pcb();

int32_t sys_exec(const uint8_t* cmd);
int32_t sys_halt(uint8_t status);

int32_t sys_open(const uint8_t* filename);
int32_t sys_close(int32_t fd);
int32_t sys_read(int32_t fd, void* buf, int32_t nbytes);
int32_t sys_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t sys_getargs(uint32_t *  buf, int32_t nbytes);
int32_t sys_vidmap(uint8_t ** screen_ptr);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);


#endif
