#include <stdio.h>
#include <string.h>
#include "munit.h"
#include "masks.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

static MunitResult test_get_missing_masks_no_file(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char missing[255];

    remove(ALL_MASKS_PATH);
    munit_assert_int(-1, ==, get_missing_masks(NULL, missing));
    
    return MUNIT_OK;
}

static MunitResult test_get_missing_masks_empty_file(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char missing[255];
    char *masks = "";
    char *expected = "\n";
    FILE *f;

    munit_assert_not_null(f = fopen(ALL_MASKS_PATH, "w"));
    fclose(f);
    munit_assert_int(0, ==, get_missing_masks(masks, missing));
    munit_assert_string_equal(missing, expected);
    
    return MUNIT_OK;
}


static MunitResult test_get_missing_masks_have_part(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char *all_masks = "dragon1\ndragon2\ndragon3\ndragon4\ndragon5\ndragon6\ndragon7\n";
    char *masks = "dragon1 dragon4 dragon7";
    char *expected = "dragon2 dragon3 dragon5 dragon6\n";
    char missing[255];
    FILE *f;
    size_t size, length;
    
    munit_assert_not_null(f = fopen(ALL_MASKS_PATH, "w"));
    size = length = strlen(all_masks);
    munit_assert_size(size = fwrite(all_masks, 1, length, f), ==, length);
    fclose(f);

    munit_assert_int(0, ==, get_missing_masks(masks, missing));
    munit_assert_string_equal(missing, expected);
    
    return MUNIT_OK;
}


static MunitResult test_get_missing_masks_have_empty(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char *all_masks = "dragon1\ndragon2\ndragon3\ndragon4\ndragon5\ndragon6\ndragon7\n";
    char *masks = "";
    char *expected = "dragon1 dragon2 dragon3 dragon4 dragon5 dragon6 dragon7\n";
    char missing[255];
    FILE *f;
    size_t size, length;
    
    munit_assert_not_null(f = fopen(ALL_MASKS_PATH, "w"));
    size = length = strlen(all_masks);
    munit_assert_size(size = fwrite(all_masks, 1, length, f), ==, length);
    fclose(f);

    munit_assert_int(0, ==, get_missing_masks(masks, missing));
    munit_assert_string_equal(missing, expected);
    
    return MUNIT_OK;
}


static MunitResult test_get_missing_masks_have_all(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char *all_masks = "dragon1\ndragon2\ndragon3\ndragon4\ndragon5\ndragon6\ndragon7\n";
    char *masks = "dragon1 dragon2 dragon3 dragon4 dragon5 dragon6 dragon7";
    char *expected = "\n";
    char missing[255];
    FILE *f;
    size_t size, length;
    
    munit_assert_not_null(f = fopen(ALL_MASKS_PATH, "w"));
    size = length = strlen(all_masks);
    munit_assert_size(size = fwrite(all_masks, 1, length, f), ==, length);
    fclose(f);

    munit_assert_int(0, ==, get_missing_masks(masks, missing));
    munit_assert_string_equal(missing, expected);
    
    return MUNIT_OK;
}


static MunitResult test_get_mask_data_no_file(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;

    char *name = "not_exist/name";

    munit_assert_int(-1, ==, get_mask_data(name, NULL));
    
    return MUNIT_OK;
}


static MunitResult test_get_mask_data_mask_exist(const MunitParameter params[], void* data)
{
    (void)params;
    (void)data;
    
    char *name = "cat";
    char path[255];
    char mydata[255];
    char *expected = "123456789\0\ndsat@utdsa\r\t\n\01234\0";
    size_t length = 30, size;
    FILE *f;

    path[0] = '\0';
    strcat(path, MASKS_DIR);
    strcat(path, name);
    strcat(path, ".png\0");
    munit_assert_not_null(f = fopen(path, "w"));
    munit_assert_size(size = fwrite(expected, 1, length, f), ==, length);
    fclose(f);

    munit_assert_int(length, ==, get_mask_data(name, mydata));
    munit_assert_memory_equal(length, mydata, expected);
    
    return MUNIT_OK;
}


static MunitTest test_suite_tests[] = {
    { (char*) "get_missing_masks - нет файла с масками ",
      test_get_missing_masks_no_file, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_missing_masks - файл пустой ",
      test_get_missing_masks_empty_file, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_missing_masks - есть только часть масок ",
      test_get_missing_masks_have_part, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_missing_masks - нету масок ",
      test_get_missing_masks_have_empty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_missing_masks - есть все маски ",
      test_get_missing_masks_have_all, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_mask_data - нет маски с таким именем ",
      test_get_mask_data_no_file, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "get_mask_data - такая маска есть ",
      test_get_mask_data_mask_exist, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    NULL
};

MunitSuite masks_suite = {
    (char*) "masks.c ",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};
