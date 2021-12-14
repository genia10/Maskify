/**
 * prepare_signals.c -- часть сервера, выполняющая подготовку
 * обработки сигналов
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"

/**
 * Действие, выполняемое обработчиком
 **/
void signals_handler()
{
    log_print("Получен сигнал прерывания терминала.");
    exit(EXIT_SUCCESS);
}


/**
 * Изменяет обработку сигналов
 **/
void prepare_signals()
{
    // Назначаем обработчик сигналов
    struct sigaction act;
    act.sa_handler = signals_handler;
    act.sa_flags = SA_ONESHOT;
    
    // Обнуляем маску сигналов
    if (sigemptyset(&act.sa_mask) < 0)
	perror_exit("Ошибка функции sigemptyset()");

    // Изменяем обработку сигнала
    if (-1 == sigaction(SIGINT, &act, NULL))
        perror_exit("Ошибка функции sigaction()");

    log_print("Настройка обработки сигналов завершена.");
}
