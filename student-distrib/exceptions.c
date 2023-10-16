#include "exceptions.h"
#include "lib.h"

void div0()         {exceptions(0);}
void res()          {exceptions(1);}
void nmi()          {exceptions(2);}
void bp()           {exceptions(3);}
void of()           {exceptions(4);}
void br()           {exceptions(5);}
void opcode()       {exceptions(6);}
void notfound()     {exceptions(7);}
void df()           {exceptions(8);}
void cso()          {exceptions(9);}
void itss()         {exceptions(10);}
void snp()          {exceptions(11);}
void ssf()          {exceptions(12);}
void gp()           {exceptions(13);}
void pf()           {exceptions(14);}
void fpu()          {exceptions(15);}
void alig()         {exceptions(16);}
void machine()      {exceptions(17);}
void simd()         {exceptions(18);}

void exceptions(int num)
{
    switch(num)
    {
        case 0:
            printf("EXCEPTION: Divide by zero");
            break;
        case 1:
            printf("EXCEPTION: Reserved");
            break;
        case 2:
            printf("EXCEPTION: NMI Interrupt");
            break;
        case 3:
            printf("EXCEPTION: Breakpoint");
            break;
        case 4:
            printf("EXCEPTION: Overflow");
            break;
        case 5:
            printf("EXCEPTION: Bound range exceeded");
            break;
        case 6:
            printf("EXCEPTION: Invalid Opcode");
            break;
        case 7:
            printf("EXCEPTION: Device not available");
            break;
        case 8:
            printf("EXCEPTION: Double Fault");
            break;
        case 9:
            printf("EXCEPTION: Coprocessor Segment Overrun");
            break;
        case 10:
            printf("EXCEPTION: Invalid TSS");
            break;
        case 11:
            printf("EXCEPTION: Segment not present");
            break;
        case 12:
            printf("EXCEPTION: Stack-segment fault");
            break;
        case 13:
            printf("EXCEPTION: General protection fault");
            break;
        case 14:
            printf("EXCEPTION: Page fault");
            break;
        case 15:
            printf("EXCEPTION: Intel reserved");
            break;
        case 16:
            printf("EXCEPTION: x87 FPU Floating-Point Error (Math Fault)");
            break;
        case 17:
            printf("EXCEPTION: Alignment Check");
            break;
        case 18:
            printf("EXCEPTION: Machine Check");
            break;
        case 19:
            printf("EXCEPTION: SIMD Floating-Point Exception");
            break;
        default:
            printf("EXCEPTION: Intel Reserved");
            break;
    }
}