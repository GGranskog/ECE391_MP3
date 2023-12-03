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

#define V_VIRTUAL   0x000B8000  // start of video in virtyal memory
#define V_IDX       0xB8        // start of video in physical memory 
#define IDX1        0x1000      // phys_mem_idx=0x1 is same as virtual_mem_idx=0x1000 
#define IDX2        0x2000      // phys_mem_idx=0x2 is same as virtual_mem_idx=0x2000
#define IDX3        0x3000      // phys_mem_idx=0x3 is same as virtual_mem_idx=0x3000

// masking bits for page-dir
#define addr3112    0xFFFFF000  // page table base addr
#define BASE_ADDR   0xFFC00000  // page base addr is bits 31-22, used for indexing into page_table
#define AVL         0x00000E00  // Available for system programmer’s use
#define G           0x00000100  // Global page (Ignored)
#define PS          0x00000080  // Page size (0 indicates 4 KBytes)
#define D           0x00000040  // Reserved (set to 0)
#define A           0x00000020  // Accessed
#define PCD         0x00000010  // Cache disabled
#define PWT         0x00000008  // Write-through
#define US          0x00000004  // User/Supervisor; 1=user/supervisor, 0=supervisor
#define RW          0x00000002  // Read/Write
#define P           0x00000001  // Present

#ifndef ASM

//void *get_physaddr(void *virtualaddr)

uint32_t page_dir[PDE]  __attribute__((aligned(PDE_SIZE)));
uint32_t page_table[PTE] __attribute__((aligned(PDE_SIZE)));
uint32_t video_mem[PTE] __attribute__((aligned(PDE_SIZE)));

void new4KBPage(uint32_t vir_address, uint32_t phys_address);

void init_page();
//void paging();
//void map_user();
//void map_page(void *physaddr, void *virtualaddr, unsigned int flags)
//void flush_tb();
void cr3();
//extern void new_video();

#endif /* ASM */

#endif /* _PAGING_H */
