#ifndef TESTS_H
#define TESTS_H


#define BUF_SIZE 128
// test launcher
void launch_tests();
int divide_by_zero();
int deref_null();
//int keyboard_test();
int paging_mr_herman();
void rtc_test();
// int rtc_test();
// void rtc_keyboard();
int frame1_read_test();
int directory_read();
int terminal_test();

int exec_read_test();
int large_read_test();
void print_filename(uint8_t* buf)

//test_terminal_read_write()

#endif /* TESTS_H */
