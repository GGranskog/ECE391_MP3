#include "idt.h"
#include "rtc.h"
#include "keyboard.h"
#include "devices.h"


/* void init_idt() 
 *  DESCRIPTION: Initialize idt
 *  INPUTS: N/A
 *  OUTPUTS: N/A
 *  SIDE EFFECTS: Changes the idt
 */
void
idt_init()
{
    int i;

    for(i = 0; i < NUM_VEC; i++)
    {
        
        idt[i].reserved0 = 0; // reserved values according to ISA
        idt[i].reserved1 = 1; // manual
        idt[i].reserved2 = 1;

        if(i >= VECTOR_INTERRUPT_START && i <= VECTOR_INTERRUPT_END)
            idt[i].reserved3 = 1;
        else
            idt[i].reserved3 = 0;

        idt[i].reserved4 = 0;
        idt[i].size = 1;
        idt[i].seg_selector = KERNEL_CS;

        if(i == VECTOR_SYS_CALL){
            idt[i].present = 1;
            idt[i].dpl = 3;
        }
        else
            idt[i].dpl = 0;
        if (i < 15){
        idt[i].present = 1; // will be set to 1 on SET_IDT_ENTRY call
        idt[i].reserved3 = 1;
        }
        else {idt[i].present = 0;}

    }


    // set idt entries to exceptions as directed in ISA manual
    SET_IDT_ENTRY(idt[0], div0);
    SET_IDT_ENTRY(idt[1], res);
    SET_IDT_ENTRY(idt[2], nmi);
    SET_IDT_ENTRY(idt[3], bp);
    SET_IDT_ENTRY(idt[4], of);
    SET_IDT_ENTRY(idt[5], br);
    SET_IDT_ENTRY(idt[6], opcode);
    SET_IDT_ENTRY(idt[7], notfound);
    SET_IDT_ENTRY(idt[8], df);
    SET_IDT_ENTRY(idt[9], cso);
    SET_IDT_ENTRY(idt[10], itss);
    SET_IDT_ENTRY(idt[11], snp);
    SET_IDT_ENTRY(idt[12], ssf);
    SET_IDT_ENTRY(idt[13], gp);
    SET_IDT_ENTRY(idt[14], pf);
    SET_IDT_ENTRY(idt[15], res);
    SET_IDT_ENTRY(idt[16], fpu);
    SET_IDT_ENTRY(idt[17], alig);
    SET_IDT_ENTRY(idt[18], machine);
    SET_IDT_ENTRY(idt[19], simd);
    idt[0x21].present = 1;
    idt[0x28].present = 1;
    SET_IDT_ENTRY(idt[0x21], keyboard_asm); //IDT ENTRY to keyboard is 0x21
    SET_IDT_ENTRY(idt[0x28], rtc_asm);    //IDT ENTRY TO rtc is 0x28
}
