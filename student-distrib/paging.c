#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "paging.h"
#include "tests.h"
#include "idt.h"

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
    page_table[1000] = V_VIRTUAL | IDX1 | RW | P;   // page_dir[1], physical(i=1) ->virtual shifter
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

/*
 *  new4KBPage
 *  Description: new 4KB page mapping for the program
 *  Inputs: vir_address and phys_address
 *  Outputs: none
 *  Return Value: new page added to dir
 */
void new4KBPage(uint32_t vir_address, uint32_t phys_address){
    uint32_t newpage_idx = vir_address/START_KERNEL; // vir address / 4MB
    page_dir[newpage_idx] = (uint32_t) page_table | US | RW | P;
    video_mem[0] = phys_address | US | RW | P;

    //TLB Flush 

    asm volatile(

        "movl %%cr3, %%eax;"
        "movl %%eax, %%cr3;"
        :
        :
        : "eax"

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

