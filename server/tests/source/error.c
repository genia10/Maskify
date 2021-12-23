#include <stdio.h>
#include <stdlib.h>
#include "munit.h"
#include "server.h"
#include "suites.h"
#include "helper.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif


extern void log_err_exit(char*);


static MunitResult test_printf_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int pid;
    char *messange_txt = "12345";
    char *expected_txt = "12345\n";

    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	printf_exit(messange_txt);
    }

    wait_child(pid, 256);    
    check_file(TMP_TXT, expected_txt);
    
    return MUNIT_OK;
}


static MunitResult test_printf_exit_2(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int pid;
    char *pattern = "It's %s\n";
    char *arg = "arg";
    char *expected_txt = "It's arg\n";

    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	printf_exit_2(pattern, arg);
    }

    wait_child(pid, 256);    
    check_file(TMP_TXT, expected_txt);

    return MUNIT_OK;
}


static MunitResult test_perror_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int pid;
    char *messange_txt = "fopen ERROR";
    char *expected_txt = "fopen ERROR: No such file or directory\n";

    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	fopen("not_exists/path", "r");
	perror_exit(messange_txt);
    }

    wait_child(pid, 256);    
    check_file(TMP_TXT, expected_txt);
    
    return MUNIT_OK;
}


static MunitResult test_log_err_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int pid;
    char *log_txt = "Text for log_file";
    char *expected_log_txt = "Text for log_file\n\n";
    char *expected_err_txt = "Программа завершилась с ошибкой. Смотрите log файл.\n";

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    
    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	log_err_exit(log_txt);
    }

    wait_child(pid, 256);    
    check_file(TMP_LOG, expected_log_txt);
    check_file(TMP_TXT, expected_err_txt);
    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "printf_exit ",
      test_printf_exit, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "printf_exit_2 ",
      test_printf_exit_2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "perror_exit ",
      test_perror_exit, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "log_err_exit ",
      test_log_err_exit, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};

MunitSuite error_suite = {
    (char*) "error.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
