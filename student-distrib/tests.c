#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "keyboard.h"
#include "file_sys.h"

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

void rtc_freq_test(){
	set_rtc_freq(2);
	int count = 0;
	while (count < 4)
	{
		count++;
	}
	count = 0;
	set_rtc_freq(4);
	count = 0;
	while (count < 8)
	{
		count++;
	}
	set_rtc_freq(8);
	count = 0;
	while (count < 16)
	{
		count++;
	}
	set_rtc_freq(16);
	count = 0;
	while (count < 32)
	{
		count++;
	}
	set_rtc_freq(32);
	count = 0;
	while (count < 64)
	{
		count++;
	}

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
void test_terminal_read_write(){
	char buf[BUF_SIZE];
	int read_num = 0;
	int write_num = 0;
	int32_t fd = NULL;
	
	/*Test for terminal read and write*/
	TEST_HEADER;
	while (1)
	{
		printf("What's your name?(press 'q' to exist) ");
		/*read in the user input to a buffer*/
		read_num = terminal_read(fd,0,  buf, BUF_SIZE);		
		/*Press q to quit*/		
		if (!strncmp("q",buf,BUF_SIZE)){
			break;
		}
		printf("Hello,");
		/*write out the buffer value to the screen*/
		write_num = terminal_write(0,buf,BUF_SIZE);
		/*compare the read in bytes and write out bytes*/
		printf("\nread in bytes: %d\nwrite out bytes: %d\n",read_num, write_num);
	}
	clear();
}

/* dir_read
 * Inputs: None
 * Outputs: Print all known files by index
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */
void dir_read(){

	clear();
	uint8_t buf[4096];
	int i;
	int file_count = 0;;

	for (i = 0; i < 63; i++){

		dir_read(0, buf, i);

	}

	return PASS;


}

/* frame1_read_test
 * Inputs: None
 * Outputs: Print all known files by index
 * Side Effects: None
 * Coverage: 
 * Files: file_sys.c, file_sys.h
 */

void frame1_read_test(){

	clear();
	uint8_t buf[174];  //buf made to support frame1 file
	int i; 
	dentry_t file;
	int rdbn = read_dentry_by_name((uint8_t*)("frame1.txt"), &file);

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
	// 



	//test_terminal_read_write();  
}
