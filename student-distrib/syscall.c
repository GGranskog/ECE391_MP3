#include "syscall.h"


int pid_stat[6] = {0,0,0,0,0,0};
int parent  = 0;
int cur_pid = 0;

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
    int pid;            // process id
    dentry_t dentry;

    int cmd_len = strlen((int8_t*)cmd);
    int file_cmd_len = 0; // length of cmd
    int file_arg_len = 0; // length of arg

    uint8_t file_cmd[STR_LEN];  // filesys for cmd
    uint8_t file_arg[STR_LEN];  // filesys for arg
    
    uint8_t  elf[4];
    uint32_t eip;
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
    pcb_t* pcb;
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
    asm volatile(
        "movl %%cr3,%%eax     ;"
        "movl %%eax,%%cr3     ;"

        : : : "eax", "cc" 
    );
    
    /* ---------------load files to mem--------------- */
    

    /* ---------------create PCB--------------- */


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

    dentry_t dentry_obj;
    int rdbn = read_dentry_by_name(fname, &dentry_obj);
    int fd;

    if(rdbn != 0){

        return -1;

    }

    if(dentry_obj.ftype == 0){

        for(fd = 2; fd < 8; fd++){

            if(!(pcb->fda[fd].eflags)){

                pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].file_pos = 0;
                 pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].eflags = 1;
                pcb->fda[fd].fop_table_ptr = &rtc_fop;
                pcb->fda[fd].ftype = 0;
                return fd;

            }
        }

    }

    if(dentry_obj.ftype == 1){

        for(fd = 2; fd < 8; fd++){

            if(!(pcb->fda[fd].eflags)){

                pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].file_pos = 0;
                 pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].eflags = 1;
                pcb->fda[fd].fop_table_ptr = &dir_fop;
                pcb->fda[fd].ftype = 1;
                return fd;

            }
        }

    }

    if(dentry_obj.ftype == 2){

        for(fd = 2; fd < 8; fd++){

            if(!(pcb->fda[fd].eflags)){

                pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].file_pos = 0;
                 pcb->fda[fd].inode = dentry_obj.inode_num;
                pcb->fda[fd].eflags = 1;
                pcb->fda[fd].fop_table_ptr = &file_fop;
                pcb->fda[fd].ftype = 2;
                return fd;

            }
        }

    }
    return -1;
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
