/**
 * prepare_socket.c -- часть сервера, выполняющая подготовку сокета
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"

/**
 * Создаёт и настраивает сокет
 **/
int getSocket(char *port)
{
    struct addrinfo *res;
    struct addrinfo hints = {
	AI_PASSIVE,              // ai_flags
	AF_INET,                 // ai_family
	SOCK_STREAM,             // ai_socktype
	0,                       // ai_protocol
	0,                       // ai_addrlen
	NULL,                    // *ai_addr
	NULL,                    // *ai_canonname
	NULL                     // *ai_next
    };

    int sd;
    
    // Получаем информацию о адресе по номеру порта
    if (getaddrinfo(NULL, port, &hints, &res) != 0)
        perror_exit("Ошибка функции getaddrinfo()");
 
    // Создаём сокет
    if (-1 == (sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)))
        perror_exit("Ошибка функции socket()");

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
	perror_exit("Ошибка функции setsocport()");

    // Присваиваем адрес сокету
    if (bind(sd, res->ai_addr, res->ai_addrlen) != 0)
        perror_exit("Ошибка функции bind()");

    // Освобождаем память
    freeaddrinfo(res);

    // Возвращаем дескриптор сокета
    return sd;
}


/**
 * Закрывает сокет
 **/
void close_socket()
{
    if (server.PID == getpid()) {
	close(server.sd);
	log_print("Сокет закрыт.");
    }
}


/**
 * Выполняет полную подготовку сокета
 **/
int prepare_socket(char *port)
{    
    // Создаём и настраиваем сокет
    int sd = getSocket(port);

    // Информируем о готовности сокета
    if (listen(sd, SOMAXCONN) < 0)
        perror_exit("Ошибка функции listen()");

    // Перед выходом закрываем сокет
    atexit(close_socket);
    
    log_print("Сокет полностью подготовлен.");

    return sd;
}
