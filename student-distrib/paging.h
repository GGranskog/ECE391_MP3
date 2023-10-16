#ifndef _PAGING_H
#define _PAGING_H

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "tests.h"
#include "idt.h"
#include "types.h"

#define PDE_SIZE        4096
#define PTE             1024 
#define PDE             1024
#define START_KERNEL    0x400000    // kernel starts @ 4 MB
#define LEVEL1          0x800000    // 1st user-level program\shell 8MB
#define VIRTUAL         0x08048000  // virtual mem of program
#define LEVEL2          0xC00000    // 2nd UL program exec by shell 12MB
#define ENTRY           0x08000000  // virtual memoray
#define OFFSET          0x048000    // offset to copy program image

#ifndef ASM

//void *get_physaddr(void *virtualaddr)

uint32_t page_dir[PDE]  __attribute__((aligned(PDE_SIZE)));
uint32_t page_table[PTE] __attribute__((aligned(PDE_SIZE)));
uint32_t video_mem[PTE] __attribute__((aligned(PDE_SIZE)));

void init_page();
//void paging();
//void map_user();
//void map_page(void *physaddr, void *virtualaddr, unsigned int flags)
//void flush_tb();
//extern void cr3();
//extern void new_video();

#endif /* ASM */

#endif /* _PAGING_H */
