/**
 * @file AdminOperation.h
 * @author Ma Xuanxuan
 * @brief 管理员操作的相关函数声明
 * @version 0.1
 * @date 2026-04-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "identity.h"

enum goods_type {
    FOOD = 1,
    COSMETICS,
    DAILY_NECESSITIES,
    DRINKS
};

typedef struct __goods {
    char name[50];
    double price;
    unsigned int remaining;
    char factory[50];
    char brand[50];
    int type;
} Good;

extern Good goods[100];
extern int GoodsCount;

#define FAIL_TO_OPEN_FILE -1
#define FAIL_TO_WRITE_FILE -2
#define OPERATION_SUCCESSFUL 0
#define TOO_MUCH_GOODS -3
#define NO_VALID_DATA -4

int AddAccount(Account account);

int ResetPassword(Account t_account);

int GetGoodsInfo();

static char* PrintGoodsType(int type);

int SaveGoodsInfo();

int SellGoods();

void ManageGoods();

void InventoryStatistics();

// TODO: 补充一个自定义的 printf 函数, 用于输出日志