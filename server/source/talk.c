#include "server.h"
#include "commands.h"
#include "masks.h"

/**
 * Считывает список масок из сокета
 * @return 0 - список успешно получен, -1 - произошла ошибка
 **/
int read_masks_list(int sd, char *masks) {
    int i = 0, size;
    char read_buf[READ_BUF_SIZE];
    
    // Получаем список масок клиента
    while ((size = read(sd, read_buf, READ_BUF_SIZE - 1)) != 0) {
        if (size == -1) return -1;
        if (read_buf[0] == '\n') break;
	masks[i++] = read_buf[0];
    }
    masks[i++] = '\0';
    return 0;
}

/**
 * Отправляет список недостающих масок
 * @return 0 - список успешно отправлен, -1 - произошла ошибка
 **/
int send_missing_masks(int sd, char *missing)
{
    int length = strlen(missing);
    
    if (write (sd, missing, length) != length)
	return -1;

    return 0;
}

/**
 * Обрабатывает запрос пользователя на список недостающих масок
 * @return 0 - запрос успешно обработан, -1 - произошла ошибка
 **/
int all_masks_request(int sd) {
    char masks[BUF_SIZE];
    char missing[BUF_SIZE];

    // Получаем список масок клиента
    if (read_masks_list(sd, masks) != 0)
	return -1;
    
    // Ожидаем команды конца списка
    if (wait_command(sd, "ENDALL") != 0)
	return -1;

    // Отправляем подтверждение получения списка
    while (send_command(sd, "OKALL"));

    // Получаем список недостающих масок
    if (get_missing_masks(masks, missing) != 0)
	return -1;

    // Отправляем список недостающих масок
    if (send_missing_masks(sd, missing) != 0)
	return -1;
    
    // Отправляем команду конца списка масок
    while(send_command(sd, "ENDALL"));

    return 0;
}

/**
 * Считывает название маски из сокета
 * @return 0 - название успешно считано, -1 - произошла ошибка
 **/
int read_mask_name(int sd, char *name)
{
    int i = 0, size;
    char read_buf[READ_BUF_SIZE];

    // Получаем название маски
    while ((size = read(sd, read_buf, READ_BUF_SIZE - 1)) != 0) {
        if (size == -1) return -1;
        if (read_buf[0] == '\n') break;
	name[i++] = read_buf[0];
    }
    name[i++] = '\0';

    return 0;
}

/**
 * Обрабатывает запрос пользователя на одну маску
 * @return 0 - запрос успешно обработан, -1 - произошла ошибка
 **/
int one_mask_request(int sd) {
    char data[BUF_SIZE];
    char name[MASK_NAME_SIZE];
    int size;

    // Получаем название маски
    if (read_mask_name(sd, name) != 0)
	return -1;
    
    // Ожидаем команды конца названия
    if (wait_command(sd, "ENDONE") != 0)
	return -1;

    // Отправляем подтверждение получения названия маски
    while (send_command(sd, "OKONE"));

    // Получаем содержимое маски
    if ((size = get_mask_data(name, data)) < 0)
	return -1;
    
    // Отправляем содержмое маски
    if (write (sd, data, size) != size)
	return -1;

    // Отправляем команду конца
    while (send_command(sd, "\nENDONE"));

    return 0;
}


/**
 * Общается с пользователем
 **/
int talk(int sd)
{
    int flag = 1;
    int again = 0;
    char command[COMMAND_SIZE];

    // Получаем и обрабатываем команды
    int code;
    while (flag == 1) {
	// Если повторить, пропускаем
	if (again == 0) {
	    // Записываем код в зависимости от результат
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
	}
	again = 0;
	
	// Производим действия в зависимости от команды
	switch (code) {
	case 0:
	    while (send_command(sd, "HELLOOK"));
	    break;
	case 1:
	    if (all_masks_request(sd) != 0) {
		code = 6; again = 1;
	    }
	    break;
	case 2:
	    if (one_mask_request(sd) != 0) {
		code = 6; again = 1;
	    }
	    break;
	case 6:
	    while (send_command(sd, "TRYAGAIN"));
	    break;
	case 7:
	    while (send_command(sd, "OKEXIT"));
	    flag = 0;
	    break;
	default:
	    while (send_command(sd, "UNKNOWN"));
	}
    }

    return 0;
}
