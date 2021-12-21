/**
 * masks.h -- содержит прототипы всех функций,
 * работающих с масками
 *
 * Copyright (c) 2021 Roman "nikanoro" Nikanorov
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef MASKS_H
#define MASKS_H

#define MASK_NAME_SIZE 20
#define ALL_MASKS_PATH "masks/all.txt"
#define MASKS_DIR "masks/"

/**
 * Получает список недостающих масок, на основе полученного списка
 * @return 0 - список успешно получен, -1 - произошла ошибка
 **/
int get_missing_masks(char *masks, char *missing);


/**
 * Получает содержимое маски
 * @return -1 - произошла ошибка, иначе - количество байт
 **/
int get_mask_data(char *name, char *data);

#endif
