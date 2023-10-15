#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "pagign.h"
#include "tests.h"
#include "idt.h"


void init_page() {

}

void paging(){

}

/* courtesy of https://wiki.osdev.org/Paging */
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

void cr3(){
    asm volatile (
    "mov %eax, page_directory
    mov %cr3, %eax
    mov eax, cr0
    or %eax, $0x80000001
    mov %cr0, %eax
    mov %eax, %cr4
    or %eax, $0x00000010
    mov cr4, eax
    ")
}


