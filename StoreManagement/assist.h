/**
 * @file assist.h
 * @author Ma Xuanxuan
 * @brief ¸¨Öúº¯ÊıµÄÉùÃ÷
 * @version 0.1
 * @date 2026-04-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <ctype.h>
#include <string.h>

#define NOT_DIGIT -1

#ifdef __cplusplus
extern "C" {
#endif

void toLower(char* str);

int str2int(const char* str);

#ifdef __cplusplus
}
#endif