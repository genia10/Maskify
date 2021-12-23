#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "munit.h"
#include "suites.h"
#include "helper.h"


void check_file(char *path, char *expected)
{
    FILE *f;
    char buf[255];
    size_t size;
    int offset = 0;

    if (strcmp(path, TMP_LOG) == 0)
	offset = 23;
    munit_assert_not_null(f = fopen(path, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_string_equal(buf + offset, expected);
    fclose(f);    
}

void wait_child(int pid, int status)
{
    int pid2;
    int real_status;

    pid2 = wait(&real_status);
    munit_assert_int(pid, ==, pid2);
    munit_assert_int(real_status, ==, status); 

}
