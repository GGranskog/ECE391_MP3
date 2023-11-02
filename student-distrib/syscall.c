#include "syscall.h"




/*
 * Exec
 * DESCRIPTION: execute commands from shell
 * INPUT:   cmd - input text from shell 
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: executes command
 */
int32_t exec(uint8_t* cmd){
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
int32_t halt(uint8_t status){
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
int32_t open(const uint8_t* fname){
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
int32_t close(int32_t fd){
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
int32_t read(int32_t fd, const void* buf, int32_t nbytes){
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
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
    return 0;
}

