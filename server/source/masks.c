#include "server.h"
#include "masks.h"


/**
 * Получает список недостающих масок, на основе полученного списка
 * @return 0 - список успешно получен, -1 - произошла ошибка
 **/
int get_missing_masks(char *masks, char *missing)
{
    FILE *f;
    char line[MASK_NAME_SIZE];
    int length;
    
    missing[0] = '\0';

    // Открываем файл со списком масок
    if ((f = fopen(ALL_MASKS_PATH, "r")) == NULL)
	return -1;

    // Накапливаем недостающие маски в missing
    while (fgets(line, MASK_NAME_SIZE, f) != NULL) {
	line[strlen(line) - 1] = '\0';
	if (strstr(masks, line) == NULL) {
	    strcat(missing, line);
	    strcat(missing, " ");
	}
    }

    // Дописываем в конец списка масок перенос строки
    if ((length = strlen(missing)) != 0) {
	missing[length - 1] = '\n';
	missing[length] = '\0';
    }
    
    return 0;
}


/**
 * Получает содержимое маски
 * @return -1 - произошла ошибка, иначе - количество байт
 **/
int get_mask_data(char *name, char *data)
{
    FILE *f;
    char path[PATH_SIZE] = "";
    int size;

    // Формируем путь к картинке
    strcat(path, MASKS_DIR);
    strcat(path, name);
    strcat(path, ".png\0");

    // Открывает файл с маской
    if ((f = fopen(path, "r")) == NULL)
	return -1;

    // Считываем маску
    if ((size = fread(data, 1, BUF_SIZE, f)) < 0)
	return -1;

    return size;
}
