#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"

#define VECTOR_INTERRUPT_START 0x20
#define VECTOR_INTTERUPT_END 0x2F
#define VECTOR_SYS_CALL 0x80

void idt_init();

#endif
