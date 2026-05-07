/**
 * @file UserOperation.h
 * @author Ma Xuanxuan
 * @brief 用户操作的相关函数声明
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

int ChangeAccountPassword(Account account, const char* NewPassword);

int BuyGoods(int type, const char* goodsName, int amount);