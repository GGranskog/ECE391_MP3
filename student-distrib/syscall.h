#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "tests.h"
#include "idt.h"
#include "types.h"
#include "filesys.h"
#include "paging.h"

#define TASK_SIZE   0x2000  // 8kB  PCB size
#define PAGE_SIZE   0x0400000 // 4MB page size
#define KER_ADDR    0x0800000  // kernel page starts addr 8MB

#define MAG_EXEC_0  0x7f
#define MAG_EXEC_1  0x45
#define MAG_EXEC_2  0x4c
#define MAG_EXEC_3  0x46

#define VIRT_ADDR VIRTUAL //virtual address

typedef struct fop_table{
    int32_t (*sys_open)(const uint8_t* fname);
    int32_t (*sys_close)(int32_t fd);
    int32_t (*sys_read)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*sys_write)(int32_t fd, const void* buf, int32_t nbytes);
}fop_table_t;

typedef struct fd_table{
    fop_table_t* fop_table_ptr;
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
}fd_table_t;

typedef struct pcb{
    fd_table_t fda[8];
    uint8_t cmd[128];
    uint8_t arg[128];
    uint32_t esp0;
    uint16_t ss0;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ss;
    uint32_t eip;
    uint32_t pid;
    uint32_t eflags;
    uint32_t cs;
    uint32_t saved_file;
    uint32_t parent_process;
    uint32_t cur_process;

}pcb_t;

fop_table_t rtc_fop;
fop_table_t dir_fop;
fop_table_t file_fop;
fop_table_t terminal_fop;



int32_t sys_exec(uint8_t* cmd);
int32_t sys_halt(uint8_t status);

int32_t sys_open(const uint8_t* fname);
int32_t sys_close(int32_t fd);
int32_t sys_read(int32_t fd, const void* buf, int32_t nbytes);
int32_t sys_write(int32_t fd, const void* buf, int32_t nbytes);


#endif
