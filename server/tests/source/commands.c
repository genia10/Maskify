#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "munit.h"
#include "commands.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif


static MunitResult test_check_command_equal_commands(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;
    
    munit_assert_true(check_command("MYCOMMAND", "MYCOMMAND"));
    return MUNIT_OK;
}


static MunitResult test_check_command_different_commands(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;
    
    munit_assert_false(check_command("MYCOMMAND", "NOTMYCOMMAND"));

    return MUNIT_OK;
}


static char* commands_params[] = {
    (char*)"HELLO", (char*)"GETALL", (char*)"GETONE",
    (char*)"EXIT", (char*)"UNKNOWN", NULL
};

static MunitParameterEnum command_to_code_params[] = {
    { (char*)"commands", commands_params },
    NULL
};


static MunitResult test_command_to_code(const MunitParameter params[], void* data)
{
    (void) data;

    const char *command;
    int code;
    
    command = munit_parameters_get(params, "commands");
    code = command_to_code(command);

    if (strcmp(command, "HELLO") == 0 && code != 0)
	return MUNIT_FAIL;
    if (strcmp(command, "GETALL") == 0 && code != 1)
	return MUNIT_FAIL;
    if (strcmp(command, "GETONE") == 0 && code != 2)
	return MUNIT_FAIL;
    if (strcmp(command, "EXIT") == 0 && code != 7)
	return MUNIT_FAIL;
    if (strcmp(command, "UNKNOWN") == 0 && code != -1)
	return MUNIT_FAIL;
	
    return MUNIT_OK;
}


static MunitResult test_send_command(const MunitParameter params[], void* data)
{
    (void) params;
    (void) data;

    int sd;
    FILE *f;
    size_t size;
    char buf[255];
    char *command = "HELLO";
    char *expected = "HELLO\n";

    sd = open(TMP_TXT, O_CREAT | O_TRUNC | O_WRONLY);
    munit_assert_int(sd, !=, -1);
    
    munit_assert_false(send_command(sd, command));
    close(sd);

    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    size = fread(buf, 1, 254, f);
    buf[size] = '\0';
    munit_assert_size(size, ==, strlen(expected)); 
    munit_assert_string_equal(buf, expected);
    fclose(f);
				    
    return MUNIT_OK;
}


static MunitResult test_get_command_correct_command(const MunitParameter params[], void* data)
{
    (void) params;
    (void) data;

    int sd;
    FILE *f;
    size_t size;
    char buf[255];
    char *my_command = "HELLO\n";
    char command[COMMAND_SIZE];
    char *expected = "HELLO";

    size = strlen(my_command);
    munit_assert_not_null(f = fopen(TMP_TXT, "w"));
    munit_assert_size(fwrite(my_command, 1, size, f), ==, size);
    fclose(f);
    
    sd = open(TMP_TXT, O_RDONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(1, ==, get_command(sd, command));
    munit_assert_string_equal(expected, command);
    close(sd);
				    
    return MUNIT_OK;
}


static MunitResult test_get_command_big_command(const MunitParameter params[], void* data)
{
    (void) params;
    (void) data;

    int sd;
    FILE *f;
    size_t size;
    char buf[255];
    char *my_command = "HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n";
    char command[COMMAND_SIZE];

    size = strlen(my_command);
    munit_assert_not_null(f = fopen(TMP_TXT, "w"));
    munit_assert_size(fwrite(my_command, 1, size, f), ==, size);
    fclose(f);
    
    sd = open(TMP_TXT, O_RDONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(0, ==, get_command(sd, command));
    close(sd);
				    
    return MUNIT_OK;
}


static MunitResult test_get_command_no_line(const MunitParameter params[], void* data)
{
    (void) params;
    (void) data;

    int sd;
    FILE *f;
    size_t size;
    char buf[255];
    char *my_command = "HELLO";
    char command[COMMAND_SIZE];

    size = strlen(my_command);
    munit_assert_not_null(f = fopen(TMP_TXT, "w"));
    munit_assert_size(fwrite(my_command, 1, size, f), ==, size);
    fclose(f);
    
    sd = open(TMP_TXT, O_RDONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(0, ==, get_command(sd, command));
    close(sd);
				    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "check_command - команды одинаковые ",
      test_check_command_equal_commands, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "check_command - команды разные ",
      test_check_command_different_commands, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "command_to_code ",
      test_command_to_code, NULL, NULL, MUNIT_TEST_OPTION_NONE, command_to_code_params },
    { (char*) "send_command ",
      test_send_command, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_command - корректная команда ",
      test_get_command_correct_command, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_command - слишком большая команда ",
      test_get_command_big_command, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_command - команда без переноса строки ",
      test_get_command_no_line, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite commands_suite = {
    (char*) "commands.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
