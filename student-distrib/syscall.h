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



int32_t exec(uint8_t* cmd);
int32_t halt(uint8_t status);

int32_t open(const uint8_t* fname);
int32_t close(int32_t fd);
int32_t read(int32_t fd, const void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);


#endif
