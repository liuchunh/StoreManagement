/**
 * @file identity.h
 * @author Ma Xuanxuan
 * @brief 身份确认代码
 * @version 0.1
 * @date 2026-04-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>

#define LOGIN_SUCCESSFULLY 1
#define WRONG_PASSWORD -1
#define UNEXISTED_ACCOUNT -7
#define NOT_ALLOWED_OPERATION -8
#define SAME_PASSWORD -9

extern const char ADMIN_ACCOUNT_NAME[];
extern const char ADMIN_PASSWORD[];

extern bool IsAdmin;

typedef struct __account {
    char accountName[100];
    char password[100];
    bool isAdmin;
} Account;

extern Account account[100];
extern int AccountCount;

void GetAccountInfo();

void GetAccountName(char* accountName);

void GetAccountPassword(char* password);

void ShowMenu(bool IsAdmin);
