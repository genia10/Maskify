/**
 * server.h -- содержит прототипы всех используемых сервером функций
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

struct {
    int sd;
    int PID;
    char *port;
    FILE *log_file;
} server;

// Основные функции
void prepare_child_processes();
void prepare_signals();
int prepare_socket(char *port);
int talk(int sd);

// Обработка ошибок
void printf_exit(char *txt);
void printf_exit_2(char *pattern, char *arg);
void perror_exit(char *txt);

// Прочие функции
void get_time_line(char *line);
void log_print(char *line);
void log_print2(char *line, char *a1);

#endif
