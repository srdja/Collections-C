#ifndef TEST_H_
#define TEST_H_

#define PASS 0
#define SKIP 77
#define ERROR 99
#define FAIL 1

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void cc_assert(bool expression, char* buff);
char *cc_msg(char *format, ...);

void cc_set_status(int s);
void cc_set_exit_on_failure(bool exit);
int cc_get_status();

#endif /* TEST_H_ */

