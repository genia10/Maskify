#include <stdio.h>
#include <stdlib.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

extern void log_err_exit(char*);


static MunitResult test_printf_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    int pid, pid2, status;
    size_t size;
    char buf[255];
    char *messange_txt = "12345";
    char *expected_txt = "12345\n";

    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	printf_exit(messange_txt);
    }
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 256); 

    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size, ==, strlen(expected_txt)); 
    munit_assert_string_equal(buf, expected_txt);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitResult test_printf_exit_2(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    int pid, pid2, status;
    size_t size;
    char buf[255];
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
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 256); 

    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size, ==, strlen(expected_txt)); 
    munit_assert_string_equal(buf, expected_txt);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitResult test_perror_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    int pid, pid2, status;
    size_t size;
    char buf[255];
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
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 256); 

    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size, ==, strlen(expected_txt)); 
    munit_assert_string_equal(buf, expected_txt);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitResult test_log_err_exit(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    int pid, pid2, status;
    size_t size;
    char buf[255];
    char *log_txt = "Text for log_file";
    char *expected_log_txt = "Text for log_file\n\n";
    char *err_txt = "Программа завершилась с ошибкой. Смотрите log файл.\n";

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    
    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	log_err_exit(log_txt);
    }
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 256); 

    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    size = size - 23;
    munit_assert_size(size, ==, strlen(expected_log_txt)); 
    munit_assert_string_equal(buf + 23, expected_log_txt);
    fclose(f);

    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size, ==, strlen(err_txt)); 
    munit_assert_string_equal(buf, err_txt);
    fclose(f);
    
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
