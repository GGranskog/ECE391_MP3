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

typedef struct fop_table{
    int32_t (*sys_open)(const uint8_t* fname);
    int32_t (*sys_close)(int32_t fd);
    int32_t (*sys_read)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*sys_write)(int32_t fd, const void* buf, int32_t nbytes);
}fop_table_t;

typedef struct fda_table{
    fop_table_t* fop_table_ptr;
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
}fda_table_t;

typedef struct pcb{
    fda_table_t[8] fda_arr;
    uint32_t esp0;
    uint16_t ss0;
    uint32_t esp;
    uint32_t flags;
    uint32_t saved_file;
    uint32_t parent;
    uint32_t child;

}pcb;

// fop_table_t open_fop;
// fop_table_t close_fop;
// fop_table_t read_fop;
// fop_table_t write_fop;



int32_t sys_exec(uint8_t* cmd);
int32_t sys_halt(uint8_t status);

int32_t sys_open(const uint8_t* fname);
int32_t sys_close(int32_t fd);
int32_t sys_read(int32_t fd, const void* buf, int32_t nbytes);
int32_t sys_write(int32_t fd, const void* buf, int32_t nbytes);


#endif
