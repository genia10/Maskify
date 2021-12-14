/**
 * prepare_child_processes.c -- часть сервера, выполняющая подготовку
 * дочерних процессов
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"

/**
 * Действие, выполняемое обработчиком
 **/
void handler()
{
    // Завершаем дочерний процесс
    while (waitpid(-1, NULL, WNOHANG) > 0);
}


/**
 * Изменяет обработку сигнала от потомков
 **/
void prepare_child_processes()
{
    // Назначаем обработчик сигналов
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;
    
    // Обнуляем маску сигналов
    if (sigemptyset(&act.sa_mask) < 0)
	perror_exit("Ошибка функции sigemptyset()");

    // Изменяем обработку сигнала
    if (-1 == sigaction(SIGCHLD, &act, NULL))
        perror_exit("Ошибка функции sigaction()");

    log_print("Настройка обработки дочерних процессов завершена.");
}
