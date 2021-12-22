#ifndef SUITES_H
#define SUITES_H

#include "munit.h"

#define TMP_TXT "tmp/tmp.txt"
#define TMP_LOG "tmp/log.log"

extern MunitSuite commands_suite;
extern MunitSuite error_suite;
extern MunitSuite funcs_suite;
extern MunitSuite masks_suite;
extern MunitSuite socket_suite;
extern MunitSuite signals_suite;
extern MunitSuite child_suite;
extern MunitSuite talk_suite;

extern MunitSuite function_suite;

#endif
