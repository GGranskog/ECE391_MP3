#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "paging.h"
#include "tests.h"
#include "idt.h"

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

#define V_VIRTUAL   0x000B8000  // start of video in virtyal memory
#define V_IDX       0xB8        // start of video in physical memory 
#define IDX1        0x1000      // phys_mem_idx=0x1 is same as virtual_mem_idx=0x1000 
#define IDX2        0x2000      // phys_mem_idx=0x2 is same as virtual_mem_idx=0x2000
#define IDX3        0x3000      // phys_mem_idx=0x3 is same as virtual_mem_idx=0x3000

/*
 *
 *  Initialize the paging setup
 *  Input:  NONE
 *  Output: NONE
 * 
 */
void init_page() {
    int i;
    for (i=0; i< PDE; i++){
        page_dir[i] = 0x82; // page not used, can read/write, and User leve/free for anyone to use
        page_table[i] = 0;  // page table entry cleared for initialization
    }
    page_table[V_IDX] = V_VIRTUAL | RW | P;         // vid physcal to virtual, RW enabled, present
    page_table[IDX1] = V_VIRTUAL | IDX1 | RW | P;   // page_dir[1], physical(i=1) ->virtual shifter
    //page_table[IDX2] = V_VIRTUAL | IDX2 | RW | P;
    //page_table[IDX3] = V_VIRTUAL | IDX3 | RW | P;


    // video memeory
    page_dir[0] = ((uint32_t)page_table) | RW | P;;
    // kernel
    page_dir[1] = START_KERNEL | PS | RW | P; //kernel starts @ x400000, size=4KB, Supervisor, RW enabled , present
    //
    //page_dir[2] = ((uint32_t)page_table) | 0x3;
    //
    //page_dir[3] = ((uint32_t)page_table) | 0x3;;

    cr3();
}


/*
 *  Enabling paging. All that is needed is to load CR3 with the address of the page directory and to set 
 *  the paging (PG) and protection (PE) bits of CR0.
 *  Input:  NONE
 *  Output: NONE
 *  effects: enables paging for 32-bit
 */
void cr3(){
    asm volatile(
    "mov %%eax, %%cr3             \n\
    mov %%cr4, %%eax             \n\
    or  $0x00000010, %%eax       \n\
    mov %%eax, %%cr4             \n\
    mov %%cr0, %%eax             \n\
    or $0x80000001, %%eax        \n\
    mov %%eax, %%cr0             \n\
    "
    :
    : "a"(page_dir)
    : "memory"
    );

}

/* courtesy of https://wiki.osdev.org/Paging 
void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    uint32_t pdindex = virtualaddr >> 22;
    uint32_t ptindex = virtualaddr >> 12 & 0x03FF;
 
    uint32_t *pd = (uint32_t *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    uint32_t *pt = ((uint32_t *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((uint32_t)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}
*/

