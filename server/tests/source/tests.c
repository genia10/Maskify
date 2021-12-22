#include <stdlib.h>
#include "munit.h"
#include "suites.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    MunitSuite unit_suites[] = {
	commands_suite, error_suite, funcs_suite, masks_suite,
	socket_suite, signals_suite, child_suite, talk_suite,
	NULL
    };
    
    MunitSuite unit_suite = {
	(char*) "", NULL, unit_suites, 1, MUNIT_SUITE_OPTION_NONE
    };

    MunitSuite function_suites[] = {
	NULL
    };

    MunitSuite function_suite = {
	(char*) "", NULL, function_suites, 1, MUNIT_SUITE_OPTION_NONE
    };

    MunitSuite type_suites[] = {
	unit_suite, function_suite, NULL
    };

    const MunitSuite test_suite = {
	(char*) "", NULL, type_suites, 1, MUNIT_SUITE_OPTION_NONE
    };
    
    return munit_suite_main(&test_suite, (void*) "", argc, argv);
}
