#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "keyboard.h"
#include "filesys.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

// test to see how our OS handles a divide by zero erorr
int divide_by_zero(){
	TEST_HEADER;
	int a = 0;
	int b;
	b = 1/a;
	return b;
}

// test to see how our OS handles a derenfrencing null erorr
int deref_null(){
	TEST_HEADER;
	int a = 0;
	int ptr;
	ptr = (int)&a;
	return ptr;
}

// test to see if our OS sets up paging correctly
int paging_mr_herman(){
	return 0;
}




/*
void rtc_keyboard(){
	keyboard_init();
	rtc_init();
}
*/
/* Checkpoint 2 tests */

/* test_terminal_read_write
 * Inputs: None
 * Outputs: Read in the keyboard inputs to a buffer and write out the buffer to the screen
 * Side Effects: None
 * Coverage: terminal_read and write, handle buffer overflow
 * Files: terminal.c, keyboard.c
 */

int terminal_test(){
    TEST_HEADER;
    //int result = PASS;
    int nbytes;
    char buf[1024];
    while(1){
        //Testing a buffer smaller than 128.
        terminal_write(0, (uint8_t*)"TESTING size 10\n", 16);
        nbytes = terminal_read(0, buf, 10);
        terminal_write(0, buf, nbytes);

        //Testing a buffer at the max buffer size
        terminal_write(0, (uint8_t*)"TESTING size 128\n", 17);
        nbytes = terminal_read(0, buf, 128);
        terminal_write(0, buf, nbytes);

        //Testing a buffer greater than the max buffer size
        terminal_write(0, (uint8_t*)"TESTING size 129\n", 17);
        nbytes = terminal_read(0, buf, 150);
        terminal_write(0, buf, nbytes);
    }
    return PASS;
}

/* dir_read
 * Inputs: None
 * Outputs: Print all known files by index
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */
int directory_read(){

	clear();
	uint8_t buf[4096];
	int i;
	//int file_count = 0;;

	for (i = 0; i < 63; i++){

		dir_read(0, buf, i);

	}

	return PASS;


}

/* frame1_read_test
 * Inputs: None
 * Outputs: Prints out contents of frame 1
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */

int frame1_read_test(){

	clear();
	uint8_t buf[174];  //buf made to support frame1 file
	int i; 
	dentry_t file;
	int rdbn = read_dentry_by_name((uint8_t*)("frame1.txt"), &file);
	// int rdbn = read_dentry_by_name((uint8_t*)("frame0.txt"), &file);

	if (rdbn == -1){

		printf("No file called frame1 in the system");

	}

	read_data(file.inode_num, 0, (uint8_t*) buf, 4096 );
	clear();

	for(i = 0; i < 174; i++){
		putc(buf[i]);

	}

	return PASS;
}




/*
 *	void rtc_test()
 *	DESCRIPTION: tests driver functions for rtc
 *	INPUTS: none
 * 	OUTPUTS: none
 *	SIDE EFFECTS: changes rtc freq
 */
void rtc_test()
{
	int i;
	int j;
	for (i = 2; i <= 1024;i *= 2)
	{
		rtc_write(i, 0, 0);
		for (j = 0; j < 2; j++)
		{
			rtc_read(0, 0, 0);
			printf("1");
		}
	
	}
	rtc_open(0);
	for (i = 0; i < 5; i++)
	{
		rtc_read(0, 0, 0);
	}
}

/* exec_read_test
 * Inputs: None
 * Outputs: Performs what the executable file is mean to
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */
int exec_read_test(){
	return PASS;
}

/* large_read_test
 * Inputs: None
 * Outputs: reads contents of a large txt file
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */
int large_read_test(){
	clear();
	uint8_t buf[174];  //buf made to support frame1 file
	int i; 
	dentry_t file;
	int rdbn = read_dentry_by_name((uint8_t*)("verylargetextwithverylongname.txt"), &file);
	// int rdbn = read_dentry_by_name((uint8_t*)("frame0.txt"), &file);

	if (rdbn == -1){

		printf("No file called frame1 in the system");

	}

	read_data(file.inode_num, 0, (uint8_t*) buf, 4096 );
	clear();

	for(i = 0; i < 174; i++){
		putc(buf[i]);

	}

	return PASS;
}







/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	// TEST_OUTPUT("deref_null", deref_null());
	// TEST_OUTPUT("divide_by_zero", divide_by_zero());
	// TEST_OUTPUT("term_driver_test", term_driver_test());  
	// TEST_OUTPUT("rtc_test", rtc_test());
	// TEST_OUTPUT("frame1_read_test", frame1_read_test());
	// TEST_OUTPUT("directory_read", directory_read());
	// TEST_OUTPUT("int exec_read_test(){", int exec_read_test());
	TEST_OUTPUT("large_read_test", large_read_test());


}
