#include "server.h"
#include "commands.h"

/**
 * Проверяет правильность команды
 * @return 1 - совпадают, 0 - разные
 **/
int check_command(const char *expect, const char *command)
{
    return strcmp(expect, command) == 0;
}


/**
 * Сопоставляет команду с её кодом
 * @return -1 - неизвестная команда, иначе - код команды
 **/
int command_to_code(const char *command)
{
    if (check_command("HELLO", command)) return 0;
    if (check_command("GETALL", command)) return 1;
    if (check_command("GETONE", command)) return 2;
    if (check_command("EXIT", command)) return 7;
    return -1;
}


/**
 * Отправляет команду пользователю
 * @return 0 - успешно отправлена, 1 - иначе
 **/
int send_command(int sd, const char *command)
{
    char my_command[COMMAND_SIZE] = "\0";
    strcat(my_command, command);
    strcat(my_command, "\n");
    int size = strlen(my_command);
    int sended_one = 0, sended_all = 0;
    do {
	sended_one = write(sd, my_command + sended_all, size - sended_all);
	if (sended_one < 0) {
	    sleep(2);
	    continue;
	}
	sended_all += sended_one;
    } while (sended_one);
    return  sended_all != size;
}


/**
 * Получает от пользователя команду
 * @return 1 - при удачном считывании, 0 - не найден конец строки,
 *        -1 - произошла ошибка  
 **/
int get_command(int sd, char *command)
{
    int size, i = 0;
    char buf[READ_BUF_SIZE];
    while ((size = read(sd, buf, READ_BUF_SIZE - 1)) != 0) {
	if (size == -1) return -1;
	if (buf[0] == '\n') {
	    command[i] = '\0';
	    return 1;
	}
	command[i++] = buf[0];
	if (i >= COMMAND_SIZE) {
	    command[--i] = '\0';
	    return 0;
	}
    }
    command[i] = '\0';
    return 0;
}


/**
 * Читает буфер и информирует пользователя, пока не найдёт нужную команду,
 * или не произойдёт ошибка
 * @return 0 - при получении ожидаемой команды, -1 - при ошибке
 **/
int wait_command(int sd, const char *expected)
{
    char command[COMMAND_SIZE];
    char scommand[COMMAND_SIZE + 9] = "EXPECTED ";
    strcat(scommand, expected);
    
    get_command(sd, command);
    while (!check_command(expected, command)) {
	while(!send_command(sd, scommand));
	get_command(sd, command);
    }
    
    return 0; 
}
