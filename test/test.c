#include "test.h"

int test_status = PASS;
int exit_on_failure = false;

const size_t msg_size = 256;
char msg_buff[256];

void cc_assert(bool exp, char *msg)
{
	 if (!exp) {
		  test_status = FAIL;
		  printf("%s\n", msg);
		  if (exit_on_failure)
			   exit(FAIL);
	 }
}

char *cc_msg(char *format, ...)
{
	 va_list args; 
	 va_start(args, format);
	 
	 vsnprintf(msg_buff, msg_size, format, args);
	 
	 va_end(args);
	 return msg_buff;
}


void cc_set_status(int status)
{
    test_status = status;
}

void cc_set_exit_on_failure(bool exit)
{
    exit_on_failure = exit;
}

int cc_get_status()
{
    return test_status;
}
