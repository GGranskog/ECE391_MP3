#include "syscall.h"




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

    uint8_t buf_check[4] = {
        0x7f, 0x45, 0x4c, 0x46              // buf for checking for magic numbers
    };
    
    uint8_t  elf;
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
    
    /* ---------------check for executables--------------- */
    if (read_dentry_by_name((uint8_t*) fname, &dentry) == -1){
        return -1;
    }

    if (read_data(dentry.inode_num, 0, buf, 4 ) == -1){   //4 for four byte
        return -1;
    }

    for(i = 0, i < 4; i++){

        if(buf[i] != buf_check[i]){
            return -1;
        }
    }

    read_data(dentry.inode_num, 24, buf, 4);       //24 for the offset of the entry
    eip = *((uint32_t * ) buf);

    /* ---------------set up paging--------------- */


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
