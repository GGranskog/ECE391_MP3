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
    int i;              // loop var
    int cmd_start = 0;  // start of cmd 
    int arg_start = 0;  // start of arg
    int str_len = strlen((const uint8_t*)cmd);
    dentry_t dentry;

    int file_cmd_length = 0;
    int file_arg_length = 0;

    uint8_t file_cmd[128];
    uint8_t file_arg[128];
    
    uint8_t elf_buf[sizeof(int32_t)];

    //Arguments for switching to user context.
    uint32_t eip_arg;
    uint32_t esp_arg;

    /* ---------------parse the args--------------- */
    for (i=0; i<128; i++){
        file_cmd[i] = '\0';
        file_arg[i] = '\0';
    }
    if (str_len == 0){return -1;}
    

    /* ---------------check for executables--------------- */
    

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
