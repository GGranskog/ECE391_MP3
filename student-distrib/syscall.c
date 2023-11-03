#include "syscall.h"


uint32_t pid_stat[6] = {0,0,0,0,0,0};
uint32_t parent  = 0;
uint32_t cur_pid = 0;

void flush_tlb();

// int32_t* terminal_fop = {0, 0, terminal_read, terminal_write};
//int32_t* rtc_fop = {rtc_open, rtc_close, rtc_read, rtc_write};
//int32_t* file_fop = {file_open, file_close, file_read, file_write};
//int32_t* dir_fop = {dir_open, dir_close, dir_read, dir_write};
//int32_t* null_fop = {null_open, null_close, null_read, null_write};

int32_t null_read(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_read(int32_t fd, const void* buf, int32_t nbytes){return -1;}

int32_t null_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes){return -1;}

int32_t null_open(const uint8_t* filename);
int32_t null_open(const uint8_t* filename){return -1;}

int32_t null_close(int32_t filename);
int32_t null_close(int32_t filename){return -1;}

/*
 * Exec
 * DESCRIPTION: execute commands from shell
 * INPUT:   cmd - input text from shell 
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: executes command
 */
int32_t sys_exec(uint8_t* cmd){
    /* ---------------define function parameters--------------- */
    int i=0;            // loop var
    int cmd_str = 0;    // start of cmd 
    int arg_idx = 0;    // start of arg
    uint32_t pid;       // process id
    dentry_t dentry;

    uint32_t cmd_len = strlen((int8_t*)cmd);
    // int file_cmd_len = 0; // length of cmd
    // int file_arg_len = 0; // length of arg

    uint8_t file_cmd[STR_LEN];  // filesys for cmd
    uint8_t file_arg[STR_LEN];  // filesys for arg
    
    uint8_t  elf[4];
    // uint32_t eip;
    uint32_t esp;
    uint32_t ebp;

    /* ---------------parse the args--------------- */
    for (i=0; i<STR_LEN; i++){
        file_cmd[i] = '\0';
        file_arg[i] = '\0';
    }
    if (cmd_len == 0){return -1;} // validate that something was typed

    for (i=0; i< cmd_len; i++){ // parsing cmd
        if (cmd[i] != ' '){ // cmd should be the first word, everything else should be an arg to the cmd
            file_cmd[i] = cmd[i];
            cmd_str++;
            // arg_str++;
        }else{
            file_cmd[i] = NULL;
            break;
        }
    }    
    for (i=cmd_str+1; i< cmd_len; i++){
        if (cmd[i] != ' '){
            for (arg_idx=cmd_str; arg_idx<cmd_len; arg_idx++){
                file_arg[arg_idx] = cmd[arg_idx];
            }
            break;
        }else{
            file_arg[arg_idx] = NULL;
            break;
        }
    }
    /* ---------------check for executables/check validity--------------- */
    // check if file does not exist
    if(read_dentry_by_name(file_cmd, &dentry)==-1){
        return -1;
    }

    // check that nothing fails during read_data
    if(read_data(dentry.inode_num, 0, elf, 4) == -1){
        return -1;
    }

    // Validate ELF as an EXECUTABLE
    if(!(elf[0] == MAG_EXEC_0 && elf[1] == MAG_EXEC_1 &&
         elf[2] == MAG_EXEC_2 && elf[3] == MAG_EXEC_3)) {
        return -1; 
    }    

    /* ---------------set up paging--------------- */
    int temp_pid = 0;
    
    for(i = 0; i < 6 ;i++){         /* find available pid, max 3 for now */
        if(pid_stat[i] == 0){
            pid_stat[i] = 1;
            cur_pid = i;                  /* set cur_pid to new one*/
            temp_pid =1;    
            break;
        }
    }
    if(temp_pid == 0){
        return -1;
    }

    // enable paging
    uint32_t addr = START_KERNEL * pid + KER_ADDR;
    page_dir[32] = addr|PS|US|RW|P;

    // flush the TLB
    flush_tlb();
    
    /* ---------------load files to mem--------------- */
    inode_t* temp_inode = (inode_t*)(node+dentry.inode_num);
    uint8_t* image_addr = (uint8_t*)VIRT_ADDR;           //it should stay the same, overwriting existing program image
    read_data(dentry.inode_num, 0, image_addr,temp_inode->length); 

    /* ---------------create PCB--------------- */
    pcb_t* pcb = (pcb_t*)(LEVEL1 - IDX2*(cur_pid+1));
    pcb->pid = cur_pid;
	pcb->esp = esp;
	pcb->ebp = ebp;
	//store the prev_pid's esp0 and ss0
	pcb->esp0 = tss.esp0;
	pcb->ss0 = tss.ss0;

    if(cur_pid != 0 ){
        pcb->parent_pid = parent;
        parent = cur_pid;
    }else{
        pcb->parent_pid = cur_pid;
    }

    for (i=0; i<8; i++){
        pcb->fda[i].fop_table_ptr = &null_fop;
        pcb->fda[i].inode = 0;
        pcb->fda[i].file_pos = 0;
        pcb->fda[i].flags = 0;
    }

    pcb->fda[0].fop_table_ptr = &terminal_fop;
    pcb->fda[0].flags = 1;

    pcb->fda[1].fop_table_ptr = &terminal_fop;
    pcb->fda[1].flags = 1;
    
    /* ---------------prep for context switch--------------- */


    /* ---------------push IRET context to kernel--------------- */


    /* ---------------IRET--------------- */


    /* ---------------return--------------- */

    return 0;
}


/*
 * Halt
 * DESCRIPTION: halts execution of program
 * INPUT:   status - status of the program
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: halts program when finished or error occurs
 */
int32_t sys_halt(uint8_t status){
    return 0;
}


/*
 * Open
 * DESCRIPTION: opens file, extension from filesys
 * INPUT:   fname - file to open
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: opens the file for reading/writing
 */
int32_t sys_open(const uint8_t* fname){
    return 0;
}


/*
 * Close
 * DESCRIPTION: closes file, extension from filesys
 * INPUT:   fd - file descriptor for closing file
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: closes the file that was read/written
 */
int32_t sys_close(int32_t fd){
    return 0;
}


/*
 * Read
 * DESCRIPTION: reads file, extension from filesys
 * INPUT:   fd - file descriptor for file read
 *          buf - buffer for 
 *          nbytes - number of bytes read
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: read a file
 */
int32_t sys_read(int32_t fd, const void* buf, int32_t nbytes){
    return 0;
}


/*
 * Write
 * DESCRIPTION: reads file, extension from filesys
 * INPUT:   fd - file descriptor for file read/write
 *          buf - buffer for 
 *          nbytes - number of bytes read
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: write to a file
 */
int32_t sys_write(int32_t fd, const void* buf, int32_t nbytes){
    return 0;
}



void flush_tlb(){
    asm volatile(
        "movl %%cr3,%%eax     ;"
        "movl %%eax,%%cr3     ;"

        : : : "eax", "cc" 
    );
}

