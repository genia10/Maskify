#include <stdio.h>
#include "munit.h"
#include "server.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

extern int read_masks_list(int, char*);

static MunitResult test_read_masks_list(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int sd;
    FILE *f;
    size_t size;
    char *masks_list = "dragon cat dog water\n";
    char masks[255];
    char *expected = "dragon cat dog water";
    char buf[255];

    size = strlen(masks_list);
    munit_assert_not_null(f = fopen(TMP_TXT, "w"));
    munit_assert_size(fwrite(masks_list, 1, size, f), ==, size);
    fclose(f);
    
    sd = open(TMP_TXT, O_RDONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(0, ==, read_masks_list(sd, masks));
    munit_assert_string_equal(expected, masks);
    close(sd);
				    
    return MUNIT_OK;
}

extern int send_missing_masks(int, char*);

static MunitResult test_send_missing_masks(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int sd;
    FILE *f;
    size_t size;
    char *missing = "dragon cat dog water robot\n";
    char buf[255];

    sd = open(TMP_TXT, O_CREAT | O_TRUNC | O_WRONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(0, ==, send_missing_masks(sd, missing));
    close(sd);
    
    munit_assert_not_null(f = fopen(TMP_TXT, "r"));
    munit_assert_size(size = fread(buf, 1, 254, f), ==, strlen(missing));
    buf[size] = '\0';
    munit_assert_string_equal(buf, missing);
    fclose(f);    
				    
    return MUNIT_OK;
}


extern int read_mask_name(int, char*);

static MunitResult test_read_mask_name(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    int sd;
    FILE *f;
    size_t size;
    char *mask_name = "dragon\n";
    char name[255];
    char *expected = "dragon";

    size = strlen(mask_name);
    munit_assert_not_null(f = fopen(TMP_TXT, "w"));
    munit_assert_size(fwrite(mask_name, 1, size, f), ==, size);
    fclose(f);
    
    sd = open(TMP_TXT, O_RDONLY);
    munit_assert_int(sd, !=, -1);    
    munit_assert_int(0, ==, read_mask_name(sd, name));
    munit_assert_string_equal(expected, name);
    close(sd);
				    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "read_masks_list ",
      test_read_masks_list, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "send_missing_masks ",
      test_send_missing_masks, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "read_mask_name ",
      test_read_mask_name, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    NULL
};


MunitSuite talk_suite = {
    (char*) "talk.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
