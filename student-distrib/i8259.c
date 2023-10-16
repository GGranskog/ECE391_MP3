/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

    // set masks for master and slave
    master_mask = 0xFF;  //0xFF means all bits are masked
    slave_mask = 0xFF;   //0xFF means all bits are masked


    // Initialize ICW1-4 on the master (control words for master)
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_DATA);
    outb(ICW3_MASTER, MASTER_8259_DATA);
    outb(ICW4, MASTER_8259_DATA);


    // Intialize ICW1-4 on the slave (control words for slave)
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA);
    outb(ICW4, SLAVE_8259_DATA);


/* // restore the masks
    outb(master_mask, MASTER_8259_DATA);
    outb(master_mask, SLAVE_8259_DATA);
*/
    enable_irq(2);  //enables master irq2 for slave
    
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {

    //checks if irq_num is within the bounds and 15 is the max for irq
    if(irq_num < 0) {return;}
    if(irq_num > 15){ return; } 

    //master IRQ
    if(irq_num < 8){  //8 is max irq num
        
       // master_mask = inb(MASTER_8259_DATA) 
       master_mask = master_mask & ~(1 << irq_num); //set bit to 0

        outb(master_mask, MASTER_8259_DATA);

        return;

    }

    //slave IRQ
   // slave_mask = inb(SLAVE_8259_DATA) 
   slave_mask = slave_mask & ~(1 << (irq_num - 8)); //8-15 is for slave
    
    outb(slave_mask, SLAVE_8259_DATA);

    return;





}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {

     //checks if irq_num is within the bounds and 15 is the max for irq
    if(irq_num < 0) {return;}
    if(irq_num > 15){ return; } 


    //master disable irq
    if(irq_num < 8){
    //   master_mask = inb(MASTER_8259_DATA);
        master_mask = master_mask |= (1 << irq_num); //set bit to 1

        outb(master_mask, MASTER_8259_DATA);
    }

    //slave disable irq
    //slave_mask = inb(SLAVE_8259_DATA);
   slave_mask = slave_mask |= (1 << (irq_num - 8)); //set bit to 1

    outb(slave_mask, SLAVE_8259_DATA);

    return;


}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {

    //checks if irq_num is within the bounds and 15 is the max for irq
    if(irq_num < 0) {return;}
    if(irq_num > 15){ return; } 

    //Master send eoi
    if(irq_num < 8){

        outb(EOI | irq_num, MASTER_8259_PORT);
        return;
    }

    //if Slave IRQ_NUM must send EOI to master port IRQ2 and slave port
    outb(EOI | (irq_num - 8), SLAVE_8259_PORT);
    outb(EOI | 2, MASTER_8259_PORT);    

}
