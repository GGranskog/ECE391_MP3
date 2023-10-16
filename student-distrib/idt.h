#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"

#define VECTOR_INTERRUPT_START 0x20
#define VECTOR_INTERRUPT_END 0x2F
#define VECTOR_SYS_CALL 0x80

void idt_init();

void div0(void);
void res(void);
void nmi(void);
void bp(void);
void of(void);
void br(void);
void opcode(void);
void notfound(void);
void df(void);
void cso(void);
void itss(void);
void snp(void);
void ssf(void);
void gp(void);
void pf(void);
void fpu(void);
void alig(void);
void machine(void);
void simd(void);

#endif
