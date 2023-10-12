#include "idt.h"
#include "x86_desc.h"

void
init_idt()
{
    int i;

    for(i = 0; i < NUM_VEC; i++)
    {
        
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;
        idt[i].reserved4 = 0;

        idt[i].size = 1;
        idt[i].seg_selector = KERNEL_CS;
        idt[i].dpl = 0;
        idt[i].present = 0;


    }
}
