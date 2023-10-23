#ifndef _FILESYS_H
#define _FILESYS_H

/*
 *  Header file for parsing the read-only file system 
 */

//#ifndef asm
#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "idt.h"
#include "types.h"

#define STR_LEN 10

typedef struct inode{
    int32_t length;
    int32_t data_block_num[1023];
}inode;

typedef struct dentry{
    int8_t fname[STR_LEN];
    int32_t ftype;
    int32_t inode_num;
    int8_t reserved[24];
}dentry_t;

typedef struct boot_block
{
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
    int8_t reserved[52];
    dentry_t direntries[63];
} boot;


void init_file_sys(const uint32_t file_sys);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


//#endif
#endif