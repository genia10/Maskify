#include "munit.h"
#include <stdlib.h>
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

static MunitResult test_compare(const MunitParameter params[], void* data) {
  const unsigned char val_uchar = 'b';
  const short val_short = 1729;
  double pi = 3.141592654;
  char* stewardesses = "stewardesses";
  char* most_fun_word_to_type;

  (void) params;
  (void) data;

  munit_assert(0 != 1);
  munit_assert_false(0);
  munit_assert_char('a', ==, 'a');
  munit_assert_uchar(val_uchar, ==, 'b');
  munit_assert_short(42, <, val_short);
  munit_assert_size(strlen("uncopyrightables"), >, strlen("dermatoglyphics"));
  munit_assert_double(pi, ==, 3.141592654);
  munit_assert_double_equal(3.141592654, 3.141592653589793, 9);
  munit_assert_string_equal(stewardesses, "stewardesses");
  munit_assert_memory_equal(7, stewardesses, "steward");
  munit_assert_memory_not_equal(8, stewardesses, "steward");
  most_fun_word_to_type = stewardesses;
  munit_assert_ptr_equal(most_fun_word_to_type, stewardesses);
  munit_assert_null(NULL);
  munit_assert_not_null(most_fun_word_to_type);
  munit_assert_ptr_equal(data, (void*)(uintptr_t)0xdeadbeef);
  return MUNIT_OK;
}

static MunitResult test_rand(const MunitParameter params[], void* user_data) {
  int random_int;
  double random_dbl;
  munit_uint8_t data[5];

  (void) params;
  (void) user_data;

  random_int = munit_rand_int_range(128, 4096);
  munit_assert_int(random_int, >=, 128);
  munit_assert_int(random_int, <=, 4096);
  random_dbl = munit_rand_double();
  munit_assert_double(random_dbl, >=, 0.0);
  munit_assert_double(random_dbl, <=, 1.0);
  munit_rand_memory(sizeof(data), data);

  return MUNIT_OK;
}

static MunitResult test_parameters(const MunitParameter params[], void* user_data) {
  const char* foo;
  const char* bar;

  (void) user_data;

  foo = munit_parameters_get(params, "foo");
  bar = munit_parameters_get(params, "bar");
  if (strcmp(foo, "one") != 0 &&
      strcmp(foo, "two") != 0 &&
      strcmp(foo, "three") != 0)
    return MUNIT_FAIL;
  if (strcmp(bar, "red") != 0 &&
      strcmp(bar, "green") != 0 &&
      strcmp(bar, "blue") != 0)
    return MUNIT_FAIL;

  return MUNIT_OK;
}

static void* compare_setup(const MunitParameter params[], void* user_data) {
  (void) params;

  munit_assert_string_equal(user_data, "µnit");
  return (void*) (uintptr_t) 0xdeadbeef;
}

static void compare_tear_down(void* fixture) {
  munit_assert_ptr_equal(fixture, (void*)(uintptr_t)0xdeadbeef);
}

static char* foo_params[] = {
  (char*) "one", (char*) "two", (char*) "three", NULL
};

static char* bar_params[] = {
  (char*) "red", (char*) "green", (char*) "blue", NULL
};

static MunitParameterEnum params[] = {
  { (char*) "foo", foo_params },
  { (char*) "bar", bar_params },
  { (char*) "baz", NULL },
  { NULL, NULL },
};


static MunitTest suite_tests[] = {
  {
    (char*) "/example/compare",
    test_compare,
    compare_setup,
    compare_tear_down,
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  { (char*) "/example/rand", test_rand, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/example/parameters", test_parameters, NULL, NULL, MUNIT_TEST_OPTION_NONE, params },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitSuite example_suite = {
  (char*) "",
  suite_tests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};
