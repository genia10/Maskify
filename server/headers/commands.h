/**
 * commands.h -- содержит прототипы всех функций,
 * обрабатвающих команды
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#define COMMAND_SIZE 20


/**
 * Проверяет правильность команды
 * @return 1 - совпадают, 0 - разные
 **/
int check_command(const char *expect, const char *command);

/**
 * Сопоставляет команду с её кодом
 * @return -1 - неизвестная команда, иначе - код команды
 **/
int command_to_code(const char *command);

/**
 * Отправляет команду пользователю
 * @return 0 - успешно отправлена, 1 - иначе
 **/
int send_command(int sd, const char *command);

/**
 * Получает от пользователя команду
 * @return 1 - при удачном считывании, 0 - не найден конец строки,
 *        -1 - произошла ошибка  
 **/
int get_command(int sd, char *command);

/**
 * Читает буфер и информирует пользователя, пока не найдёт нужную команду,
 * или не произойдёт ошибка
 * @return 0 - при получении ожидаемой команды, -1 - при ошибке
 **/
int wait_command(int sd, const char *expected);

#endif
