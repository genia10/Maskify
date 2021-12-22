#include <stdio.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

extern void signals_handler();

static MunitResult test_signals_handler(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char buf[255];
    char *expected = "Получен сигнал прерывания терминала.\n";
    size_t size;
    FILE *f;
    int pid, pid2, status;

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    
    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	signals_handler();
    }
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 0); 
    fclose(server.log_file);
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitResult test_prepare_signals(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char buf[255];
    char *expected = "Настройка обработки сигналов завершена.\n";
    size_t size;
    FILE *f;

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    prepare_signals();
    fclose(server.log_file);
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "signals_handler ",
      test_signals_handler, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "prepare_signals ",
      test_prepare_signals, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite signals_suite = {
    (char*) "prepare_signals.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
