#include "filesys.h"


/*
 *  initialize file system
 *  Input:  fname - name of the file, dentry - 
 *  Output: none
 */
void init_file_sys(uint32_t file_sys){

}

/*
 *  Pass dentry with the file name, file type, and inode number for the file, then return 0
 *  Input:  fname - name of the file, dentry - 
 *  Output: retuns 0 if success, else -1 on failure
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    uint32_t i;
    uint32_t rdbi = read_dentry_by_index(i, dentry);
    return rdbi;
}

/*
 *  Pass dentry with the file name, file type, and inode number for the file, then return 0
 *  Input:  index - , dentry -
 *  Output: retuns 0 if success, else -1 on failure
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    return -1;
}

/*
 *  Only check that the given inode is within the valid range
 *  Input:  inode - number of inodes, offset - starting point as on offset from file start, 
 *          buffer - , length - length from offset EOF in bytes
 *  Output: retuns returns the number of bytes read and placed in the buffer, 
 *          0 means all bytes were read and EOF was reached, -1 means failure
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    int i;
    for (i = 0; i < length; i++){

    }
    return -1;
}



/*
 * dir_open
 * DESCRIPTION: Opens a directory. sets the index of dentry
 * INPUT: none
 * OUTPUT: none
 * RETURN: 0 for success, -1 for fail
 * SIDE AFFECTS: 
 */
int32_t dir_open (const char* file_name){

    return -1;
}


/*
 * dir_close
 * DESCRIPTION: Close a directory
 * INPUT: none
 * OUTPUT: none
 * RETURN: 0 for success, -1 for fail
 * SIDE AFFECTS: 
 */
int32_t dir_close (int32_t fd){

    return 0;
}


/*
 * dir_read
 * DESCRIPTION: Reads a file name from the directory, which then reads the length of that file in bytes (length) and
                fills the buf with its contents
 * INPUT: buf - buffer that will be filled
          length - num of bytes of the file that is read
 * OUTPUT: none
 * RETURN: length of file
 * SIDE AFFECTS: 
 */
int32_t dir_read (int32_t fd, uint8_t* buf, uint32_t length){

    return -1;
}


/*
 * dir_write
 * DESCRIPTION: not used read only system
 * INPUT: none
 * OUTPUT: none
 * RETURN: -1
 * SIDE AFFECTS: 
 */
int32_t dir_write (int32_t fd, uint8_t* buf, uint32_t length){

    return -1;
}


/*
 * file_open
 * DESCRIPTION: Opens a file that matches file_name
 * INPUT: file_name - name of file that is gonna be opened
 * OUTPUT: none
 * RETURN: 0 for success, -1 for fail
 * SIDE AFFECTS: 
 */
int32_t file_open (const char* file_name){

    return -1;
}


/*
 * file_close
 * DESCRIPTION: Closes a file that matches file_name
 * INPUT: none
 * OUTPUT: none
 * RETURN: 0 for success, -1 for fail
 * SIDE AFFECTS: 
 */
int32_t file_close (int32_t fd){

    return 0;
}


/*
 * file_read
 * DESCRIPTION: reads the length (num of bytes of file)
 * INPUT: buf - used to fill from reading the file
          length - num of bytes copied from the file
 * OUTPUT: none
 * RETURN: length of file read
 * SIDE AFFECTS: 
 */
int32_t file_read (int32_t fd, uint8_t* buf, uint32_t length){

    return -1;
}


/*
 * file_write
 * DESCRIPTION: not used - read only file
 * INPUT: none
 * OUTPUT: none
 * RETURN: -1
 * SIDE AFFECTS: 
 */
int32_t file_write (int32_t fd, uint8_t* buf, uint32_t length){

    return -1;
}



