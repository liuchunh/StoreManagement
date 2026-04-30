#define _CRT_SECURE_NO_WARNINGS

#include "AdminOperation.h"
#include "assist.h"

/*
 * @brief 向 account.csv 文件中添加一个账号
 * 
 * @param account 要添加的账号信息
 * @retval 执行状态
 * 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (FAIL_TO_WRITE_FILE) 表示文件写入失败
*/
int AddAccount(Account t_account) {
    /*FILE* fp = fopen("account.csv", "a+");
    if (fp == NULL) {
        printf("Error Occurred when opening file.\n");
        return FAIL_TO_OPEN_FILE;
    }
    char buffer[256];
    sprintf(buffer, "\n%s,%s", account.accountName, account.password);
    if (fwrite(buffer, sizeof(char), strlen(buffer), fp) != strlen(buffer)) {
        fclose(fp);
        return FAIL_TO_WRITE_FILE;
    }
    fclose(fp);*/

	memcpy(account[AccountCount].accountName, t_account.accountName, sizeof(t_account.accountName));
	memcpy(account[AccountCount].password, t_account.password, sizeof(t_account.password));
	AccountCount++;

	SaveAccountInfo();
    return 0;
}

/*
 * @brief 将内存中的账号信息保存到 account.csv 文件中
 * 
 * @retval 执行状态
 * 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (FAIL_TO_WRITE_FILE) 表示文件写入失败
*/
int SaveAccountInfo() {
    FILE* fp = fopen("account.csv", "w");
    if (fp == NULL) {
        printf("Error Occurred when opening file.\n");
        return FAIL_TO_OPEN_FILE;
    }
    for (int i = 0; i < AccountCount; i++) {
        char buffer[256];
        sprintf(buffer, "%s,%s\n", account[i].accountName, account[i].password);
        if (fwrite(buffer, sizeof(char), strlen(buffer), fp) != strlen(buffer)) {
            fclose(fp);
            return FAIL_TO_WRITE_FILE;
        }
    }
    fclose(fp);
    return 0;
}

/**
 * @brief 重置用户密码
 * 
 * @param Account t_account 要重置密码的账号信息
 * @retval 执行状态
 * 
 * @note 返回值: 0 表示成功, -1 (UNEXISTED_ACCOUNT) 表示账号不存在
 */
int ResetPassword(Account t_account) {
    bool AccountFound = false;
    for (int i = 0; i < AccountCount; i++) {
        if (strcmp(account[i].accountName, t_account.accountName) == 0) {
			memset(account[i].password, 0, sizeof(account[i].password));
			printf("请输入新密码：");
            GetAccountPassword(account[i].password);
            AccountFound = true;
            break;
        }
	}
    if (!AccountFound) {
        printf("账号不存在!\n");
        return UNEXISTED_ACCOUNT;
    }

	return 0;
}

/**
 * @brief 从 goodsinfo.csv 文件中读取商品信息并存储到内存中
 * 
 * @return int 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (TOO_MUCH_GOODS) 表示商品数量过多, -3 (NO_VALID_DATA) 表示没有有效数据
 */
int GetGoodsInfo() {
    FILE* fp = fopen("goodsinfo.csv", "r");
    if (fp == NULL) {
        // printf("Error Occurred when opening file.\n");
        return FAIL_TO_OPEN_FILE;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (GoodsCount >= 100) {
            return TOO_MUCH_GOODS;
        }
        Good good = { 0 };
        char type[20] = { 0 };
        int matched = sscanf(buffer, " %49[^,], %lf, %u, %49[^,], %49[^,], %s",
                             good.name,
                             &good.price,
                             &good.remaining,
                             good.factory,
                             good.brand,
                             type);
        // 表头过滤
        toLower(good.name);
        if (strcmp(good.name, "name") == 0) {
            continue;
        }

        if (matched == EOF) {
            // printf("Warning: Failed to parse line, skipping line: %s", buffer);
            continue;
        }
        if (matched != 6) {
            // printf("Warning: Line format is incorrect, skipping line: %s", buffer);
            continue;
        }

        /*
        printf("Parsed good: Name=%s, Price=%.2f, Remaining=%u, Factory=%s, Brand=%s, Type=%s\n",
               good.name,
               good.price,
               good.remaining,
               good.factory,
               good.brand,
               type); // 调试输出，查看解析的商品信息
        */

        toLower(type);
        if (strcmp(type, "food") == 0) {
            good.type = FOOD;
        }
        else if (strcmp(type, "cosmetics") == 0) {
            good.type = COSMETICS;
        }
        else if (strcmp(type, "daily_necessities") == 0) {
            good.type = DAILY_NECESSITIES;
        }
        else if (strcmp(type, "drinks") == 0) {
            good.type = DRINKS;
        }
        else {
            // printf("Warning: Invalid type '%s' for good '%s', skipping line: %s", type, good.name, buffer);
            continue; // 无效的类型，跳过
        }

        goods[GoodsCount] = good;
        GoodsCount++;
    }
    if (GoodsCount == 0) {
        // printf("Warning: No valid goods found in file.\n");
        return NO_VALID_DATA;
    }
    fclose(fp);
    return true;
}

/**
 * @brief 将内存中的商品信息保存到 goodsinfo.csv 文件中
 * 
 * @return int 
 * 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (FAIL_TO_WRITE_FILE) 表示文件写入失败
 */
int SaveGoodsInfo() {
	FILE* fp = fopen("goodsinfo.csv", "w");
    if (fp == NULL) {
        printf("Error Occurred when opening file.\n");
        return FAIL_TO_OPEN_FILE;
	}

    for (int i = 0; i < GoodsCount; i++) {
        char type[20] = { 0 };
        switch (goods[i].type) {
            case FOOD:
                strcpy(type, "Food");
                break;
            case COSMETICS:
                strcpy(type, "Cosmetics");
                break;
            case DAILY_NECESSITIES:
                strcpy(type, "Daily_Necessities");
                break;
            case DRINKS:
                strcpy(type, "Drinks");
                break;
            default:
                strcpy(type, "Unknown");
                break;
        }
        char buffer[256];
        if (goods[i].remaining == 0) {
            continue; // 跳过库存为0的商品 不保存到文件中
		}
        sprintf(buffer, "%s,%.2f,%u,%s,%s,%s\n",
                goods[i].name,
                goods[i].price,
                goods[i].remaining,
                goods[i].factory,
                goods[i].brand,
                type);
        if (fwrite(buffer, sizeof(char), strlen(buffer), fp) != strlen(buffer)) {
            fclose(fp);
            return FAIL_TO_WRITE_FILE;
        }
    }
    fclose(fp);
	return 0;
}

/**
 * @brief 针对商品类型转换成字符串
 * 
 * @param type 
 * @return char* 
 */
char* PrintGoodsType(int type) {
    switch (type) {
        case FOOD:
            return "Food";
        case COSMETICS:
            return "Cosmetics";
        case DAILY_NECESSITIES:
            return "Daily Necessities";
        case DRINKS:
            return "Drinks";
        default:
            return "Unknown";
    }
    return "";
}

/**
 * @brief 出售商品
 * 
 * @return int 
 * @note 返回值: true (1) 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (TOO_MUCH_GOODS) 表示商品数量过多, -3 (NO_VALID_DATA) 表示没有有效数据, false (0) 表示输入无效
 */
int SellGoods() {
    int result = GetGoodsInfo();
    if (result == FAIL_TO_OPEN_FILE) {
        // printf("打开文件失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
        return FAIL_TO_OPEN_FILE;
    }
    else if (result == TOO_MUCH_GOODS) {
        // printf("商品数量过多，文件中只读取了前100个商品. 当前数量: %d\n", GoodsCount);
		return TOO_MUCH_GOODS;
    }
    else if (result == NO_VALID_DATA) {
        // printf("文件中没有有效的商品数据!\n");
        return NO_VALID_DATA;
	}
    else {
        printf("商品信息读取成功. \n总商品数量: %d\n", GoodsCount);
        for (int i = 0; i < GoodsCount; i++) {
            if (goods[i].remaining == 0) {
                continue; // 跳过库存为0的商品
            }
            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
				   i + 1,
                   goods[i].name,
                   goods[i].price,
                   goods[i].remaining,
                   goods[i].factory,
                   goods[i].brand,
                   PrintGoodsType(goods[i].type)
            );
        }
    }

    char str[4] = { 0 };
    int sign = 0, count = 0;
	printf("请输入要出售的商品编号: ");
    scanf("%s", str);
	sign = str2int(str);
    
    // 防呆
    if (sign == NOT_DIGIT) { // 输入的不是数字
        printf("输入的编号无效! 请输入一个有效的数字编号.\n");
		return false;
    }
    if (sign < 1 || sign > GoodsCount) {
        printf("输入的编号无效! 请输入一个介于 1 和 %d 之间的编号.\n", GoodsCount);
        return false;
	}

    printf("请输入要出售的数量: ");
	scanf("%s", str);
    count = str2int(str);
    if (count == NOT_DIGIT) { // 输入的不是数字
        printf("输入的数量无效! 请输入一个有效的数字数量.\n");
        return false;
    }
    if (count < 1 || count > goods[sign - 1].remaining) {
        printf("输入的数量无效! 请输入一个介于 1 和 %u 之间的数量.\n", goods[sign - 1].remaining);
        return false;
    }
    if (goods[sign - 1].remaining < count) {
        printf("库存不足! 当前剩余数量: %u\n", goods[sign - 1].remaining);
        return false;
	}
    goods[sign - 1].remaining -= count;
    printf("出售成功! 商品: %s, 数量: %d, 剩余: %u\n", goods[sign - 1].name, count, goods[sign - 1].remaining);
	
    // 因为我们不是 GUI 程序 没有办法在出售后 收到 WM_QUIT / WM_CLOSE 消息来保存数据
	// 所以我们只能在每次出售后 直接调用 SaveGoodsInfo 函数 来保存数据了
    // 这样虽然会增加一些磁盘 I/O 的开销 但是可以保证数据的安全性 不会因为程序异常退出而导致数据丢失
    if (SaveGoodsInfo() != 0) {
        printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
        return false;
	}
    return true;
}

void ManageGoods() {
    return;
}

void InventoryStatistics() {
    return;
}