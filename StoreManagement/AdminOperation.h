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
    double remaining;
    char factory[50];
    char brand[50];
    int type;
} Good;

#define FAIL_TO_OPEN_FILE -1
#define FAIL_TO_WRITE_FILE -2

int AddAccount(Account account);

void ResetPassword(Account t_account);

void SellGoods();

void ManageGoods();

void InventoryStatistics();

void SaveGoodsInfo();

void LoadGoodsInfo();