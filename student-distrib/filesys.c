#include "filesys.h"



/*
 *  initialize file system
 *  Input:  fname - name of the file, dentry - 
 *  Output: none
 */


void init_file_sys(uint32_t file_sys){
    boot_block = (boot_t*)file_sys;
    node = (inode_t*)(file_sys + BLOCK_SIZE);
    data_block = (boot_block->inode_count + 1)*BLOCK_SIZE + file_sys;
}

/*
 *  Pass dentry with the file name, file type, and inode number for the file, then return 0
 *  Input:  fname - name of the file, dentry - 
 *  Output: retuns 0 if success, else -1 on failure
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    uint32_t n;
    int32_t i;
    int len = STR_LEN;
    if (fname == NULL || dentry->fname == NULL || strlen((int8_t*)fname) > len) {return -1;}
    for (i=0; i<boot_block->dir_count; i++){
        n = strncmp((int8_t*)fname, (int8_t*)boot_block->direntries[i].fname, len);
        if (n==0){
            read_dentry_by_index(i, dentry);
            return 0;
        }
    }
  
  
    return -1;
}


/*
 * iterate through fname by maxfile name
 check if its not null
 if dir file name is != 
 *
 *
 */


/*
 *  Pass dentry with the file name, file type, and inode number for the file, then return 0
 *  Input:  index - , dentry -
 *  Output: retuns 0 if success, else -1 on failure
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if (index >= boot_block->dir_count){return -1;}

    strncpy((int8_t*)dentry->fname, boot_block->direntries[index].fname, STR_LEN);
    dentry->ftype = boot_block->direntries[index].ftype;
    dentry->inode_num = boot_block->direntries[index].inode_num;
    return 0;
}

/*
 *  Only check that the given inode is within the valid range
 *  Input:  inode - number of inodes, offset - starting point as on offset from file start, 
 *          buffer - , length - length from offset EOF in bytes
 *  Output: retuns returns the number of bytes read and placed in the buffer, 
 *          0 means all bytes were read and EOF was reached, -1 means failure
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    uint32_t i, data_start, data_idx;
    uint8_t* copy_idx;

    data_start = offset / BLOCK_SIZE;
    data_idx = offset % BLOCK_SIZE;

    if (node[inode].data_block_num[data_start] >= boot_block->data_count || inode >= boot_block->inode_count){return -1;}

    copy_idx = (uint8_t*)(data_block + (node[inode].data_block_num[data_start])*BLOCK_SIZE + data_idx);

    for (i=0; i<length;i++){
        if (data_idx > BLOCK_SIZE){
            data_idx = 0;
            data_start += 1;

            if (node[inode].data_block_num[data_start] >= boot_block->data_count){return -1;}
        
            copy_idx = (uint8_t*)(data_block + node[inode].data_block_num[data_start]*BLOCK_SIZE);
        }
        if (node[inode].length <= (i + offset)){return i;}

        buf[i] = *copy_idx;
        copy_idx++;
        data_idx++;
    }

    return i;
}



/*
 * dir_open
 * DESCRIPTION: Opens a directory. sets the index of dentry
 * INPUT: none
 * OUTPUT: none
 * RETURN: 0 for success, -1 for fail
 * SIDE AFFECTS: 
 */
int32_t dir_open (const uint8_t* filename){

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
int32_t dir_read (int32_t fd, void* buf, int32_t nbytes){

    if (dentry_index == -1){                //if index is -1 then file is not open

        return -1;

    }

    if (buf == NULL){                       

        return -1;

    }

    if (dentry_index >= boot_block->dir_count){         // if at the end of the array

        return 0;

    }
    int dir_length;                         //length of the file
    char* file_name;                        //file name

    char buf_string[STR_LEN];               
    int idx, i;                             //index loops

    if (dir_length < nbytes){
        dir_length = STR_LEN;           //  make sure within bounds 
    }
    else{

        dir_length = nbytes;

    }

    file_name = boot_block->direntries[dentry_index++].fname;           // file name in dentry

    for(idx = 0; i < dir_length; idx++){

        buf_string[i] = file_name[i];                       //copying into the string bufer

    }

    strncpy((int8_t*) buf, (int8_t*)buf_string, dir_length);            

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
int32_t dir_write (int32_t fd, const void* buf, int32_t nbytes){

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
int32_t file_open (const uint8_t* filename){

    dentry_t dentry;            //temp dentry
    uint32_t rdbi = read_dentry_by_name((uint8_t*) filename, &dentry);    //call read dentry by name
    if (rdbi != 0){                 // if not 0 that means no file

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
int32_t file_read (int32_t inode_count, void* buf, int32_t nbytes){

    if (buf == NULL){

        return -1;

    }
    return read_data(inode_count, 0, buf, nbytes);         

}


/*
 * file_write
 * DESCRIPTION: not used - read only file
 * INPUT: none
 * OUTPUT: none
 * RETURN: -1
 * SIDE AFFECTS: 
 */
int32_t file_write (int32_t fd, const void* buf, int32_t nbytes){

    return -1;

}



