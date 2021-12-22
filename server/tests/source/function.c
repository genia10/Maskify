#include <stdio.h>
#include <unistd.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

static MunitResult test_just_hello(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int pid, pid2, pid3;

    switch (pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	execl("/bin/echo", "echo", "cd .. && make && ./server 9876 | /bin/bash", (char *)0);
    default:
	sleep(2);
	switch (pid2 = fork()) {
	case -1:
	    return MUNIT_FAIL;
	case 0:
	    execl("/bin/echo", "echo", "-e \"HELLO\nEXIT\n\" | nc kappa.cs.petrsu.ru 9876 > 1.txt", (char *)0);
	    exit(EXIT_SUCCESS);
	default:
	        sleep(2);
		kill(pid, SIGINT);
		pid3 = wait(NULL);
		munit_assert_int(pid, ==, pid3);
		kill(pid2, SIGINT);
		pid3 = wait(NULL);
		munit_assert_int(pid2, ==, pid3);
	}
    }
    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "send HELLO and EXIT ",
      test_just_hello, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite function_suite = {
    (char*) "functional.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
