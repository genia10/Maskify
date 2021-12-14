#include "server.h"

#define READ_BUF_SIZE 2
#define BUF_SIZE 4000000
#define COMMAND_SIZE 20
#define MASK_NAME_SIZE 20
#define ALL_MASKS_PATH "masks/all.txt"
#define MASKS_DIR "masks/"


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
 * @return 1 - успешно отправлена, 0 - иначе
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
    return  sended_all == size;
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
 * Читает буфер и нформирует пользователя, пока не найдёт нужную команду,
 * или не произойдёт ошибка
 * @return 1 - при получении ожидаемой команды, 0 - при ошибке
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
    
    return 1; 
}


/**
 * Обрабатывает запрос пользователя на список всех масок
 **/
void send_missing_masks(int sd) {
    char read_buf[READ_BUF_SIZE];
    char buf[BUF_SIZE];
    char line[MASK_NAME_SIZE];
    char masks[BUF_SIZE];
    int size, i;

    // Получаем список масок клиента
    i = 0;
    while ((size = read(sd, read_buf, READ_BUF_SIZE - 1)) != 0) {
        if (size == -1) perror_exit("Ошибка функции read() в функцие send_missing_masks()");
        if (read_buf[0] == '\n') break;
	masks[i++] = read_buf[0];
    }
    masks[i++] = '\0';

    // Ожидаем команды конца списка
    wait_command(sd, "ENDALL");

    // Отправляем подтверждение получения списка
    while (!send_command(sd, "OKALL"));

    // Получаем список недостающих масок
    buf[0] = '\0';
    FILE *f = fopen(ALL_MASKS_PATH, "r");
    while (fgets(line, MASK_NAME_SIZE, f) != NULL) {
	line[strlen(line) - 1] = '\0';
	if (strstr(masks, line) == NULL) {
	    strcat(buf, line);
	    strcat(buf, " ");
	}
    }
    int length = strlen(buf);
    if (length != 0) {
	buf[length - 1] = '\n';
	
	// Отправляем список недостающих масок
	if (write (sd, buf, length) != length)
	    perror_exit("Ошибка функции write() в функцие send_missing_masks()");
    }
    
    // Отправляем команду конца списка масок
    while(!send_command(sd, "ENDALL"));
}


/**
 * Обрабатывает запрос пользователя на одну маску
 **/
void send_one_mask(int sd) {
    char read_buf[READ_BUF_SIZE];
    char buf[BUF_SIZE];
    char mask[MASK_NAME_SIZE];
    int size, i;

    // Получаем название маски
    i = 0;
    while ((size = read(sd, read_buf, READ_BUF_SIZE - 1)) != 0) {
        if (size == -1) perror_exit("Ошибка функции read() в функцие send_missing_masks()");
        if (read_buf[0] == '\n') break;
	mask[i++] = read_buf[0];
    }
    mask[i++] = '\0';

    // Ожидаем команды конца названия
    wait_command(sd, "ENDONE");

    // Отправляем подтверждение получения названия маски
    while (!send_command(sd, "OKONE"));

    // Получаем содержимое маски
    buf[0] = '\0';
    strcat(buf, MASKS_DIR);
    strcat(buf, mask);
    strcat(buf, ".png\0");
    FILE *f = fopen(buf, "r");
    size = fread(buf, 1, BUF_SIZE, f);
    
    // Отправляем содержмое маски
    if (write (sd, buf, size) != size)
	perror_exit("Ошибка функции write() в функцие send_missing_masks()");

    // Отправляем команду конца
    while (!send_command(sd, "\nENDONE"));
}


/**
 * Общается с пользователем
 **/
int talk(int sd)
{
    int flag = 1;
    char command[COMMAND_SIZE];

    // Получаем и обрабатываем команды
    int code;
    while (flag == 1) {
	switch (get_command(sd, command)) {
	case 1:
	    code = command_to_code(command);
	    break;
	case 0:
	    code = 8;
	    break;
	default:
	    code = 6;
	}
	switch (code) {
	case 0:
	    while (!send_command(sd, "HELLOOK"));
	    break;
	case 1:
	    send_missing_masks(sd);
	    break;
	case 2:
	    send_one_mask(sd);
	    break;
	case 6:
	    while (!send_command(sd, "ERROR"));
	    flag = 0;
	    break;
	case 7:
	    while (!send_command(sd, "OKEXIT"));
	    flag = 0;
	    break;
	default:
	    while (!send_command(sd, "UNKNOWN"));
	}
    }

    return 0;
}
