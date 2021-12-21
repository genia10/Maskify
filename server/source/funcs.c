/**
 * funcs.c -- содержит функции общего назначения
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"


/**
 *  Возвращает текущее время в виде:
 *  [DD.MM.YYYY, hh:mm:ss] (22 symbols)
 **/
 void get_time_line(char *line)
 {
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    sprintf(line, "[%02d.%02d.%04d, %02d:%02d:%02d]", 
        local->tm_mday, local->tm_mon + 1, 1900 + local->tm_year,
        local->tm_hour, local->tm_min, local->tm_sec);
 }

/**
 *  Записывает строку в log файл
 **/
void log_print(char *line)
{
    char time[23];
    get_time_line(time);
    fprintf(server.log_file, "%s %s\n", time, line);
}

/**
 *  Записывает строку в log файл
 **/
void log_print2(char *line, char *a1)
{
    char time[23];
    get_time_line(time);
    fprintf(server.log_file, "%s %s%s\n", time, line, a1);
}
