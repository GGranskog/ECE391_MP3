#include "syscall.h"
#include "sys_asm.h"


#define FILE_ARRAY_SIZE 8
#define USER_SPACE_START  0x8000000
#define USER_SPACE_END    0x8400000
#define UESP  0x0083FFFFC // 128MB+4MB-4
#define EFLAG 0x200
#define FAIL -1

uint32_t pid_stat[6] = {0,0,0,0,0,0};
uint32_t parent  = 0;
uint32_t cur_pid = 0;


void flush_tlb();

int32_t null_read(int32_t fd, void* buf, int32_t nbytes);
int32_t null_read(int32_t fd, void* buf, int32_t nbytes){return -1;}

int32_t null_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes){return -1;}

int32_t null_open(const uint8_t* filename);
int32_t null_open(const uint8_t* filename){return -1;}

int32_t null_close(int32_t filename);
int32_t null_close(int32_t filename){return -1;}

/*
 * Exec
 * DESCRIPTION: execute commands from shell
 * INPUT:   cmd - input text from shell 
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: executes command
 */
int32_t sys_exec(const uint8_t* cmd){
    /* ---------------define function parameters--------------- */
    int i=0;            // loop var
    int cmd_str = 0;    // start of cmd 
    int arg_idx = 0;    // start of arg
    uint32_t pid;       // process id
    dentry_t dentry;

    uint32_t cmd_len = strlen((int8_t*)cmd);
    // int file_cmd_len = 0; // length of cmd
    // int file_arg_len = 0; // length of arg

    uint8_t file_cmd[STR_LEN];  // filesys for cmd
    uint8_t file_arg[STR_LEN];  // filesys for arg
    
    uint8_t  elf[4];
    uint32_t eip;
    uint32_t esp;
    uint32_t ebp = 0;

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
    if(read_data(dentry.inode_num, 0, elf, 4) == -1){return -1;}

    // Validate ELF as an EXECUTABLE
    if(!(elf[0] == MAG_EXEC_0 && elf[1] == MAG_EXEC_1 &&
         elf[2] == MAG_EXEC_2 && elf[3] == MAG_EXEC_3)) {return -1;}    

    /* ---------------set up paging--------------- */
    int temp_pid = 0;
    
    for(i = 0; i < 6 ;i++){         /* find available pid, max 3 for now */
        if(pid_stat[i] == 0){
            pid_stat[i] = 1;
            cur_pid = i;                  /* set cur_pid to new one*/
            temp_pid =1;    
            break;
        }
    }
    if(temp_pid == 0){ // pid is full
        return -1;
    }

    // enable paging
    uint32_t addr = START_KERNEL * pid + KER_ADDR;
    page_dir[32] = addr|PS|US|RW|P;

    // flush the TLB
    flush_tlb();
    
    /* ---------------load files to mem--------------- */
    inode_t* temp_inode = (inode_t*)(node+dentry.inode_num);
    uint8_t* image_addr = (uint8_t*)VIRT_ADDR;           //it should stay the same, overwriting existing program image
    read_data(dentry.inode_num, 0, image_addr,temp_inode->length); 

    /* ---------------create PCB--------------- */
    pcb_t* pcb = get_pcb();
    if (pcb == NULL){return -1;}
    pcb->pid = cur_pid;
	// pcb->esp = esp;
	// pcb->ebp = ebp;
	//store the prev_pid's esp0 and ss0
	// pcb->esp0 = tss.esp0;
	// pcb->ss0 = tss.ss0;

    if(cur_pid != 0 ){
        pcb->parent_pid = parent;
        parent = cur_pid;
    }else{
        pcb->parent_pid = cur_pid;
    }

    for (i=0; i<8; i++){
        pcb->fda[i].fop_table_ptr = &null_fop;
        pcb->fda[i].inode = 0;
        pcb->fda[i].file_pos = 0;
        pcb->fda[i].flags = 0;
    }

    pcb->fda[0].fop_table_ptr = &terminal_fop;
    pcb->fda[0].flags = 1;
    pcb->fda[0].file_pos = 0;
    pcb->fda[0].inode = 0;

    pcb->fda[1].fop_table_ptr = &terminal_fop;
    pcb->fda[1].flags = 1;
    pcb->fda[1].file_pos = 0;
    pcb->fda[1].inode = 0;
    strncpy((int8_t*)pcb->arg, (int8_t*)(file_arg), STR_LEN);

    /* ---------------prep for context switch--------------- */
    uint8_t eip_buf[4];
    read_data(dentry.inode_num, 24, eip_buf, 4); 
    eip = *((int*)eip_buf);

    esp = ENTRY + PAGE_SIZE - 4;
    pcb->eip = eip;
    pcb->esp = esp;
    tss.ss0 = KERNEL_DS;
    tss.esp0 = KER_ADDR - (TASK_SIZE*cur_pid) - 4;
    pcb->esp0 = tss.esp0;

    uint32_t ESP_asm, EBP_asm;
    asm volatile(
        "movl %%esp, %0 ;"
        "movl %%ebp, %0 ;"
        : "=r" (ESP_asm) ,"=r" (EBP_asm) 
    );

    pcb->task_ebp = EBP_asm;    
    pcb->task_esp = ESP_asm;
    // sti();

    /* ---------------push IRET context to kernel--------------- */
    // push order:
    // User_DS, ESP, EFlags, User_CS, EIP
        asm volatile(
        "movw  %%ax, %%ds;"
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushfl  ;"
        "pushl %%ecx;"
        "pushl %%edx;"
        :
        : "a"(USER_DS), "b"(esp), "c"(USER_CS), "d"(eip)
        : "cc", "memory"
    );
    asm volatile("IRET");

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
    // cli();
    pcb_t* pcb = get_pcb();
    if (pcb == NULL){return -1;}
    int i;
    for (i=0; i< 8; i++){
        pcb->fda[i].file_pos = 0;
        pcb->fda[i].flags = 0;
        pcb->fda[i].fop_table_ptr = NULL;
        pcb->fda[i].inode = 0;
    }
    if (pcb->pid < 1){
        sys_exec((uint8_t*)"shell");
    }
    pid_stat[cur_pid] = 0;
    cur_pid = pcb->parent_pid;
    pcb_t* parent_pcb = get_pcb();
    parent = parent_pcb->parent_pid;

    int32_t esp, ebp;
    esp = pcb->esp;
    ebp = pcb->eip;
    // esp = pcb->task_esp;
    // ebp = pcb->task_ebp;

    uint32_t addr = (cur_pid*START_KERNEL)+KER_ADDR;
    page_dir[32] = addr|PS|US|RW|P;
    flush_tlb();
    
    tss.ss0 = KERNEL_DS;
    // tss.esp0 = KER_ADDR - (TASK_SIZE * cur_pid) - 4;
    tss.esp0 = parent_pcb->esp0;
    // tss.ss0 = pcb->ss0;

    int32_t retval = status & 0xFF;  
    // sti();
    asm volatile(
        "movl %0, %%esp ;"
        "movl %1, %%ebp ;"
        "movl %2, %%eax ;"
        "leave;"
        "ret;"
        : 
        : "r" (esp), "r" (ebp), "r"(retval)
        : "esp", "ebp", "eax"
    );
    return status;
}


/*
 * Open
 * DESCRIPTION: opens file, extension from filesys
 * INPUT:   fname - file to open
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: opens the file for reading/writing
 */
int32_t sys_open(const uint8_t* filename){


    int fd=-1;
    int i;
    dentry_t cur_dentry;


    /* if the filename is NULL fail */
    if (filename == NULL){
        printf("read NULL!");
        return FAIL;
    }
    
    pcb_t* pcb = get_pcb();
    if( (int)filename < USER_SPACE_START || (int)filename > USER_SPACE_END -4 ){return FAIL;}
    /* select the table that is not in use */
    for(i = 0; i < FILE_ARRAY_SIZE; i++ ){
        if(pcb->fda[i].flags == 0){
            fd =i;
            break;
        }
    }
    /* if all is in use, return fail*/
    if(fd == -1){
        printf("open too many files!");
        return FAIL;
    }

    /*fail if not exist*/
    if( -1 == read_dentry_by_name(filename,&cur_dentry) ){
        printf("no such file");
        return FAIL; 
    };

    /* set up entry value in corresponding fd */
    //pcb->fda[fd].fop_table_ptr = &fop_table[cur_dentry.file_type];
    pcb->fda[fd].file_pos = 0;
    pcb->fda[fd].flags = 1;
    /* if it is rtc, set it to -1 */
    if (cur_dentry.ftype == 0){
        pcb->fda[fd].inode = -1;
    }else{pcb->fda[fd].inode = cur_dentry.inode_num;}
    
    /* if corresponding action fails, we shoudl fail too */
    if(FAIL == pcb->fda[fd].fop_table_ptr->sys_open(filename)){
        // file_array[fd].flags = 0;    
        return FAIL;
    }
    /* return fd for success */
    return fd;
    // pcb_t* pcb;
    // dentry_t dentry_obj;
    // int rdbn = read_dentry_by_name(fname, &dentry_obj);
    // int fd;

    // if(rdbn != 0){

    //     return -1;

    // }

    // if(dentry_obj.ftype == 0){

    //     for(fd = 2; fd < 8; fd++){

    //         if(!(pcb->fda[fd].flags)){

    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].file_pos = 0;
    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].flags = 1;
    //             pcb->fda[fd].fop_table_ptr = &rtc_fop;
    //             //pcb->fda[fd].ftype = 0;
    //             return fd;

    //         }
    //     }

    // }

    // if(dentry_obj.ftype == 1){

    //     for(fd = 2; fd < 8; fd++){

    //         if(!(pcb->fda[fd].flags)){

    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].file_pos = 0;
    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].flags = 1;
    //             pcb->fda[fd].fop_table_ptr = &dir_fop;
    //             //pcb->fda[fd].ftype = 1;
    //             return fd;

    //         }
    //     }

    // }

    // if(dentry_obj.ftype == 2){

    //     for(fd = 2; fd < 8; fd++){

    //         if(!(pcb->fda[fd].flags)){

    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].file_pos = 0;
    //             pcb->fda[fd].inode = dentry_obj.inode_num;
    //             pcb->fda[fd].flags = 1;
    //             pcb->fda[fd].fop_table_ptr = &file_fop;
    //             // pcb->fda[fd].ftype = 2;
    //             return fd;

    //         }
    //     }

    // }
    // return -1;
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

 /* not valid idx, try to close default, or originally closed, fail */ 
    pcb_t* pcb = get_pcb();
    if(fd <= 1 || fd>=FILE_ARRAY_SIZE || pcb->fda[fd].flags == 0){
        return FAIL;
    }
    /* close and call corresponding close */
    pcb->fda[fd].flags = 0;
    return pcb->fda[fd].fop_table_ptr->sys_close(fd);


    // pcb_t* pcb;
    // if (fd < 2){   //start of file idx

    //     return -1;
    // }

    // if (fd > 7){   //max file idx - 1

    // return -1;
        
    // }  

    // if (pcb->fda[fd].flags == 0){
    //     return -1;
    // }

    // pcb->fda[fd].inode = 0;
    // pcb->fda[fd].file_pos = 0;
    // pcb->fda[fd].flags = 1;
    // pcb->fda[fd].fop_table_ptr = &null_fop;
    // //pcb->fda[fd].fop_table_ptr.close(fd);

    // return 0;

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
int32_t sys_read(int32_t fd, void* buf, int32_t nbytes){
    int32_t ret;
    /* judge whether the ptr is in user space */
    pcb_t* pcb = get_pcb();
    if( (int)buf < USER_SPACE_START || (int)buf + nbytes > USER_SPACE_END -4 ){return -1;}
    if (nbytes <= 0){return -1;}
    if (fd < 0 || fd >= FILE_ARRAY_SIZE || fd == 1){return -1;}
    if (!pcb->fda[fd].flags){return -1;} 
    if (pcb->fda[fd].fop_table_ptr->sys_read == NULL){return -1;}



    /* Call the corresponding read function base on the file type */
    ret = (*(pcb->fda[fd].fop_table_ptr->sys_read))(fd,buf,nbytes);
    return ret;
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
    pcb_t* pcb = get_pcb();
    if (fd<=0 || fd >= FILE_ARRAY_SIZE){return -1;}
    if (!pcb->fda[fd].flags || buf == NULL){return -1;}
    if (pcb->fda[fd].fop_table_ptr->sys_write == NULL){return -1;}

    /* judge whether the ptr is in user space */
    if( (int)buf < USER_SPACE_START || (int)buf + nbytes > USER_SPACE_END -4 ){return -1;}

    /* Call the corresponding write function base on the file type */
    return ((pcb->fda[fd].fop_table_ptr->sys_write)(fd,buf,nbytes));
}


/*
 * get_pcb
 * DESCRIPTION: grabs the pcb for the current process
 * INPUT:   NONE
 * OUTPUT:  NONE
 * RETURN:  returns pcb of current pid
 */
pcb_t* get_pcb(){
    return (pcb_t*)(LEVEL1- TASK_SIZE*(cur_pid+1));
}


/*   
 *  fop_init
 *  DESCRIPTION: initialize all of the FOP tables for each of the drivers
 *  INPUT: NONE
 *  OUTPUT: NONE
 */
void fop_init(){
    terminal_fop.sys_close  = terminal_close;
    terminal_fop.sys_open   = terminal_open;
    terminal_fop.sys_read   = terminal_read;
    terminal_fop.sys_write  = terminal_write;

    rtc_fop.sys_close   = rtc_close;
    rtc_fop.sys_open    = rtc_open;
    rtc_fop.sys_read    = rtc_read;
    rtc_fop.sys_write   = rtc_write;

    file_fop.sys_close  = file_close;
    file_fop.sys_open   = file_open;
    file_fop.sys_read   = file_read;
    file_fop.sys_write  = file_write;

    dir_fop.sys_close   = dir_close;
    dir_fop.sys_open    = dir_open;
    dir_fop.sys_read    = dir_read;
    dir_fop.sys_write   = dir_write;

    null_fop.sys_close  = null_close;
    null_fop.sys_open   = null_open;
    null_fop.sys_read   = null_read;
    null_fop.sys_write  = null_write;
}


/*
 * flush_tlb
 * DESCRIPTION: flufhes the tlb when change in address space
 * INPUT:   none
 * OUTPUT:  none
 * RETURN:  0
 * SIDE AFFECTS: flushes the tlb
 */
void flush_tlb(){
    asm volatile(
        "movl %%cr3,%%eax     ;"
        "movl %%eax,%%cr3     ;"

        : : : "eax", "cc" 
    );
}

