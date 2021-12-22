#include <stdio.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

extern void handler();

static MunitResult test_handler(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    switch(fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	exit(EXIT_SUCCESS);
    }
    handler();
    
    return MUNIT_OK;
}


static MunitResult test_prepare_child_processes(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char buf[255];
    char *expected = "Настройка обработки дочерних процессов завершена.\n";
    size_t size;
    FILE *f;

    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    prepare_child_processes();
    fclose(server.log_file);
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);
    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "handler ",
      test_handler, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "prepare_childs_processes ",
      test_prepare_child_processes, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite child_suite = {
    (char*) "prepare_child_processes.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
