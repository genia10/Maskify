/**
 * error.c -- содержит функции, выполняющие проверку ошибок
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"


/**
 * Выводит сообщение с ошибкой и завершает программу
 **/
void printf_exit(char *txt)
{
    fprintf(stderr, "%s\n", txt);
    exit(EXIT_FAILURE);
}

/**
 * Выводит сообщение с ошибкой и завершает программу
 **/
void printf_exit_2(char *pattern, char *arg)
{
    fprintf(stderr, pattern, arg);
    exit(EXIT_FAILURE);
}


/**
 * Выводит сообщение с ошибкой и завершает программу
 **/
void perror_exit(char *txt)
{
    perror(txt);
    exit(EXIT_FAILURE);
}


/**
 * Записывает сообщение в лог и завершает программу
 **/
void log_err_exit(char *msg)
{
    char time[23];
    get_time_line(time);
    fprintf(server.log_file, "%s %s\n\n", time, msg);
    fprintf(stderr, "Программа завершилась с ошибкой. Смотрите log файл.\n");
    exit(EXIT_FAILURE);
}
