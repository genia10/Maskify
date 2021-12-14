/**
 * server.c -- сервер для проекта по ТППО Masfiky
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#include "server.h"

#define BUF_SIZE 255


/**
 *  Записывает в лог конец программы и закрывает его
 **/
void print_end()
{
    if (server.PID == getpid()) {
	log_print("Программа завершена.");
	fclose(server.log_file);
    }
}


/**
 *  Выводит Ip-адрес и порт, откуда пришло соединение
 **/
void print_client_info(struct sockaddr_in *sinp)
{
    char buf[INET_ADDRSTRLEN];
    const char *addr;
    
    // Преобразуем адрес в текстовую строку
    if (NULL == (addr = inet_ntop(AF_INET, &sinp->sin_addr, buf, INET_ADDRSTRLEN)))
        perror_exit("inet_ntop ERROR");

    // Выводим адрес и порт
    char log_buf[BUF_SIZE];
    sprintf(log_buf, "Новый клиент - %s:%d", addr, ntohs(sinp->sin_port));
    log_print(log_buf);
}


/**
 * Точка входа
 **/
int main(int argc, char *argv[])
{
    // Устанавливаем порт
    if (argc == 2) server.port = argv[1];
    else server.port = "7777";
    server.PID = getpid();

    // Настраиваем логирование
    if ((server.log_file = fopen("logs.log", "w")) == NULL)
        perror_exit("Ошибка функции fopen()");
    setvbuf(server.log_file, NULL, _IONBF, 0);

    // Выводим сообщение о успешном запуске программы
    char log_buf[BUF_SIZE];
    sprintf(log_buf, "Программа успешно запущена PID(%d).", server.PID);
    log_print(log_buf);
    atexit(print_end);
    
    // Подготавливаем обработку сигналов и дочерние процессы
    prepare_signals();
    prepare_child_processes();

    // Подготавливаем сокет
    server.sd = prepare_socket(server.port);
    
    // Обслуживаем соединения
    while(1) {
        struct sockaddr addr;
        socklen_t addrLen = sizeof(addr);
        int newSd;	
	
        // Принимаем запрос
        if ((newSd = accept(server.sd, &addr, &addrLen)) < 0)
            continue;

	// Выводим информацию о клиенте 
        print_client_info((struct sockaddr_in *) &addr);
	
        // Создаём дочерний процесс
        pid_t pid;
        switch(pid = fork()) {
	// Если произошла ошибка
	case -1:
	    log_print("Не удалось выделить процесс для клиента.");
	    break;
	    
	// Если это дочерний процесс
	case 0:
            // Закрываем ненужные сокеты
            close(server.sd);
            fclose(server.log_file);
	    
            // Общаемся с пользователем
            talk(newSd);

	    // Завершаем процесс
	    close(newSd);
	    exit(EXIT_SUCCESS);
	    break;

	// Если это родительский процесс
	default:
	    // Закрываем сокет клиента
            close(newSd);
	    break;
        }
    }

    return 0;
}

