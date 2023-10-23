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
    return -1;
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

