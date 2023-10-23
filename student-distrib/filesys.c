#include "filesys.h"


    inode_t node;
    dentry_t dentry;
    boot_t boot;
    int dentry_index; 
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
    if (rdbi > 1){return 0;}
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
    uint32_t i, idx, byte=0;
    for (i = 0; i < length; i++){
        idx = i + offset;
        idx += byte;
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

    dentry_index = 0;
    return -0;

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

    if (dentry_index == -1;){

        return -1;

    }

    if (buf == NULL){

        return -1;

    }

    if (dentry_index >= boot_block->dir_count){

        return 0;

    }
    int dir_length;
    char* file_name;

    char buf_string[STR_LEN];
    int idx;

    if (dir_length < length){
        dir_length = STR_LEN;
    }
    else{

        dir_length = length;

    }

    file_name = boot_block->direntries[dentry_index++].fname;

    for(idx = 0; i < dir_length; idx++){

        buf_string[i] = file_name[i];

    }

    strncopy((int8_t*) buf, (int8_t*)buf_string, dir_length);

    return dir_length;

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

    dentry_t dentry;
    uint32_t rdbi = read_dentry_by_name((uint8_t*) file_name, &dentry)
    if (rdbi != 0){

        return -1;

    }
    else if (dentry.ftype != 2){            // 2 is the file type

        return -1;

    }
    else if (dentry.inode_num >= boot_block->inode_count){

        return -1;

    }
    
    return 0;

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
 * INPUT: 
 * OUTPUT: none
 * RETURN: length of file read
 * SIDE AFFECTS: 
 */
int32_t file_read (int32_t inode_count, uint32_t offset, uint8_t* buf, uint32_t length){

    if (buf == NULL){

        return -1

    }
    return read_data(inode_count, offset, (uint8_t*) buf, length);

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



