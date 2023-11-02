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

#define STR_LEN 32
#define D_ENT_SIZE 24
#define BOOT_SIZE 52
#define ENTRY_SIZE 63   
#define INODE_LEN 1023

#define BLOCK_SIZE  4096
#define LEN         4


typedef struct inode{
    int32_t length;
    int32_t data_block_num[INODE_LEN];
}inode_t;

typedef struct dentry{
    int8_t fname[STR_LEN];
    int32_t ftype;
    int32_t inode_num;
    int8_t reserved[D_ENT_SIZE];
}dentry_t;

typedef struct boot_block
{
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
    int8_t reserved[BOOT_SIZE];
    dentry_t direntries[ENTRY_SIZE];
} boot_t;

// uint32_t global_idx =0;
inode_t* node;
dentry_t d_ent;
boot_t* boot_block;
int dentry_index; 
uint32_t data_block;
    

void init_file_sys(uint32_t file_sys);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t dir_open (const char* file_name);
int32_t dir_close (int32_t fd);
int32_t dir_read (int32_t fd, uint8_t* buf, uint32_t length);
int32_t dir_write (int32_t fd, uint8_t* buf, uint32_t length);

int32_t file_open (const char* file_name);
int32_t file_close (int32_t fd);
int32_t file_read (int32_t inode_count, uint32_t offset, uint8_t* buf, uint32_t length);
int32_t file_write (int32_t fd, uint8_t* buf, uint32_t length);




//#endif
#endif

