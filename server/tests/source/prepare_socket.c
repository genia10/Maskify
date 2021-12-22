#include <stdio.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

extern int getSocket(char*);

static MunitResult test_get_socket(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int sd;
    int pid, pid2, status;
    
    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	freopen(TMP_TXT, "w", stderr);
	sd = getSocket("9876");
	munit_assert_int(sd, >, 2);
	exit(EXIT_SUCCESS);
    }
    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 0); 
    
    return MUNIT_OK;
}


extern void close_socket();


static MunitResult test_close_socket_parent(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    char buf[255];
    char *expected = "Сокет закрыт.\n";
    size_t size;
    
    server.PID = getpid();
    server.sd = getSocket("9876");
    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    close_socket();
    fclose(server.log_file);
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size - 23, ==, strlen(expected)); 
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);

    return MUNIT_OK;
}


static MunitResult test_close_socket_child(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    char buf[255];
    char *expected = "";
    size_t size;
    int pid, pid2, status;
    
    server.PID = getpid();
    server.sd = getSocket("9876");

    switch(pid = fork()) {
    case -1:
	return MUNIT_FAIL;
    case 0:
	munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
	close_socket();
	fclose(server.log_file);
	exit(EXIT_SUCCESS);
    }

    pid2 = wait(&status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(status, ==, 0); 
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf, expected);
    fclose(f);

    return MUNIT_OK;
}


static MunitResult test_prepare_socket(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    FILE *f;
    char buf[255];
    char *expected = "Сокет полностью подготовлен.\n";
    size_t size;
    
    munit_assert_not_null(server.log_file = fopen(TMP_LOG, "w"));
    munit_assert_int(prepare_socket("9876"), >, 2);
    fclose(server.log_file);
    
    munit_assert_not_null(f = fopen(TMP_LOG, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf + 23, expected);
    fclose(f);

    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "get_socket ",
      test_get_socket, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "close_socket - закрывает родитель ",
      test_close_socket_parent, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "close_socket - закрывает потомок ",
      test_close_socket_child, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "prepare_socket ",
      test_prepare_socket, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite socket_suite = {
    (char*) "prepare_socket.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
