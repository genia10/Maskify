#include <stdio.h>
#include <time.h>
#include <math.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

static MunitResult test_get_time_line(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char line[23];
    int num;;
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    get_time_line(line);

    line[3] = '\0';    
    num = atoi(line + 1);
    munit_assert_int(num, >=, 0);
    munit_assert_int(num, <, 32);
    munit_assert_int(abs(local->tm_mday - num), <=, 0);

    line[6] = '\0';    
    num = atoi(line + 4);
    munit_assert_int(num, >=, 1);
    munit_assert_int(num, <, 13);
    munit_assert_int(abs(local->tm_mon + 1 - num), <=, 0);

    line[11] = '\0';    
    num = atoi(line + 7);
    munit_assert_int(num, >=, 0);
    munit_assert_int(num, <, local->tm_year + 1901);
    munit_assert_int(abs(local->tm_year + 1900 - num), <=, 0);

    line[15] = '\0';    
    num = atoi(line + 13);
    munit_assert_int(num, >=, 0);
    munit_assert_int(num, <, 25);
    munit_assert_int(abs(local->tm_hour - num), <=, 0);

    line[18] = '\0';    
    num = atoi(line + 16);
    munit_assert_int(num, >=, 0);
    munit_assert_int(num, <, 60);
    munit_assert_int(abs(local->tm_min - num), <=, 0);

    line[21] = '\0';    
    num = atoi(line + 19);
    munit_assert_int(num, >=, 0);
    munit_assert_int(num, <, 60);
    munit_assert_int(abs(local->tm_sec - num), <=, 1);
    
    return MUNIT_OK;
}


static MunitResult test_log_print(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    size_t size;
    char buf[255];
    char *messange = "Text for log_file";
    char *expected = "Text for log_file\n";

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    log_print(messange);
    fclose(server.log_file);

    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    size = size - 23;
    munit_assert_size(size, ==, strlen(expected)); 
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);

    return MUNIT_OK;
}


static MunitResult test_log_print2(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    size_t size;
    char buf[255];
    char *messange = "Text for log_file - ";
    char *messange2 = "this text";
    char *expected = "Text for log_file - this text\n";

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    log_print2(messange, messange2);
    fclose(server.log_file);

    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    size = size - 23;
    munit_assert_size(size, ==, strlen(expected)); 
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);

    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "get_time_line ",
      test_get_time_line, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "log_print ",
      test_log_print, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "log_print2 ",
      test_log_print2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};

MunitSuite funcs_suite = {
    (char*) "funcs.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
