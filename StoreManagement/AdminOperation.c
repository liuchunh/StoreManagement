#define _CRT_SECURE_NO_WARNINGS

#include "AdminOperation.h"
#include "assist.h"
#include "NodeList.h"

/*
 * @brief 向 account.csv 文件中添加一个账号
 * 
 * @param account 要添加的账号信息
 * @retval 执行状态
 * 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (FAIL_TO_WRITE_FILE) 表示文件写入失败
*/
int AddAccount(Account t_account) {
	memcpy(account[AccountCount].accountName, t_account.accountName, sizeof(t_account.accountName));
	memcpy(account[AccountCount].password, t_account.password, sizeof(t_account.password));
	AccountCount++;

	SaveAccountInfo();
    return OPERATION_SUCCESSFUL;
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
    return OPERATION_SUCCESSFUL;
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
    char LAccountName[100] = { 0 };
    memcpy(LAccountName, t_account.accountName, sizeof(t_account.accountName));
    toLower(LAccountName); 

    bool AccountFound = false;

    for (int i = 0; i < AccountCount; i++) {
        char LAccountNameFile[100] = { 0 };
        strcpy(LAccountNameFile, account[i].accountName);
        toLower(LAccountNameFile);

		// 账号名不区分大小写 但是密码区分大小写
        if (strcmp(LAccountNameFile, LAccountName) == 0) {
			memset(account[i].password, 0, sizeof(account[i].password));
			// 重置密码为默认密码
            strcpy(account[i].password, DEFAULT_PASSWORD);
            AccountFound = true;
            break;
        }
	}
    if (!AccountFound) {
        return UNEXISTED_ACCOUNT;
    }

	return OPERATION_SUCCESSFUL;
}

/**
 * @brief 从 goodsinfo.csv 文件中读取商品信息并存储到内存中
 * 
 * @return int 
 * @note 返回值: 0 表示成功, -1 (FAIL_TO_OPEN_FILE) 表示文件打开失败, -2 (TOO_MUCH_GOODS) 表示商品数量过多, -3 (NO_VALID_DATA) 表示没有有效数据
 */
int GetGoodsInfo(Node** head) {
    FILE* fp = fopen("goodsinfo.csv", "r");
    if (fp == NULL) {
        printf("Error Occurred when opening file.\n");
        return FAIL_TO_OPEN_FILE;
    }
    if (head != NULL) { // 重新读取数据时 先清空链表
        Clear(head);
        GoodsCount = 0;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (GoodsCount >= 100) {
            return TOO_MUCH_GOODS;
        }
        Node* data = (Node*) malloc(sizeof(Node));
        if (data == NULL) {
			return NO_VALID_DATA; // 内存分配失败 可能是因为没有足够的内存了
        }
        char type[20] = { 0 };
        int matched = sscanf(buffer, " %49[^,], %lf, %u, %49[^,], %49[^,], %s",
                             data->value.name,
                             &data->value.price,
                             &data->value.remaining,
                             data->value.factory,
                             data->value.brand,
                             type);
        char nameLower[50];
        strcpy(nameLower, data->value.name);
        toLower(nameLower);
        if (strcmp(nameLower, "name") == 0) {
            free(data);
            continue;
        }

        if (matched == EOF) {
            free(data);
            continue;
        }
        if (matched != 6) {
            free(data);
            continue;
        }

        data->value.sign = GoodsCount + 1;
        toLower(type);
        if (strcmp(type, "food") == 0) {
            data->value.type = FOOD;
        }
        else if (strcmp(type, "cosmetics") == 0) {
            data->value.type = COSMETICS;
        }
        else if (strcmp(type, "daily_necessities") == 0) {
            data->value.type = DAILY_NECESSITIES;
        }
        else if (strcmp(type, "drinks") == 0) {
            data->value.type = DRINKS;
        }
        else {
            free(data);
            continue;
        }

        Insert(head, data->value);
        GoodsCount++;
        free(data);
    }
    fclose(fp);
    return OPERATION_SUCCESSFUL;
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
    fprintf(fp, "Name, price, remaining, factory, brand, type\n");
    Node* node = head;
    while (node != NULL) {
        char type[20] = { 0 };
        switch (node->value.type) {
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
        char buffer[256] = { 0 };
        if (node->value.remaining == 0) {
            node = node->next;
            continue; // 跳过库存为0的商品 不保存到文件中
        }
        // write good info to file
        sprintf(buffer, "%s, %.2f, %u, %s, %s, %s\n",
                node->value.name,
                node->value.price,
                node->value.remaining,
                node->value.factory,
                node->value.brand,
                type);
        if (fwrite(buffer, sizeof(char), strlen(buffer), fp) != strlen(buffer)) {
            fclose(fp);
            return FAIL_TO_WRITE_FILE;
        }
        node = node->next;
    }
    fclose(fp);
    return OPERATION_SUCCESSFUL;
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
    memset(goods, 0, sizeof(Good));
    int result = GetGoodsInfo(&head);
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
		Node* CurrentNode = head;
        while (CurrentNode != NULL) {
            if (CurrentNode->value.remaining == 0) {
                Node* temp = CurrentNode;
                CurrentNode = CurrentNode->next;
				Delete(head, temp->value); // 删除库存为0的商品
				// free(temp); 节点已经在 Delete 函数中释放了 这里不需要再释放一次了
                continue; // 跳过库存为0的商品 不显示在售卖列表中
            }
            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                   CurrentNode->value.sign,
                   CurrentNode->value.name,
                   CurrentNode->value.price,
                   CurrentNode->value.remaining,
                   CurrentNode->value.factory,
                   CurrentNode->value.brand,
                   PrintGoodsType(CurrentNode->value.type)
            );
            CurrentNode = CurrentNode->next;
		}
        // free(CurrentNode);
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

    Node* TargetGood = NULL;
	Node* node = head;
    while (node != NULL) {
        if (sign == node->value.sign) {
			TargetGood = node;
			break;
		}
		node = node->next;
    }

    //free(node);
    if (TargetGood == NULL) {
        printf("未找到编号为 %d 的商品!\n", sign);
        // free(TargetGood);
        return NO_VALID_DATA;
    }

	// 因为 TargetGood 是一个指针 所以我们直接修改 TargetGood->value 就可以修改链表中的商品信息了
	int remaining = TargetGood->value.remaining;

    printf("请输入要出售的数量: ");
	scanf("%s", str);
    count = str2int(str);
    if (count == NOT_DIGIT) { // 输入的不是数字
        printf("输入的数量无效! 请输入一个有效的数字数量.\n");
        // free(TargetGood);
        return false;
    }
    if (count < 1 || count > remaining) {
        printf("输入的数量无效! 请输入一个介于 1 和 %u 之间的数量.\n", remaining);
        // free(TargetGood);
        return false;
    }
    if (remaining < count) {
        printf("库存不足! 当前剩余数量: %u\n", remaining);
        // free(TargetGood);
        return false;
	}
    TargetGood->value.remaining -= count;
    printf("出售商品 %s 共 %d 个, 剩余 %u 个\n", TargetGood->value.name, count, TargetGood->value.remaining);

    // free(TargetGood);

    // 因为我们不是 GUI 程序 没有办法在出售后 收到 WM_QUIT / WM_CLOSE 消息来保存数据
	// 所以我们只能在每次出售后 直接调用 SaveGoodsInfo 函数 来保存数据了
    // 这样虽然会增加一些磁盘 I/O 的开销 但是可以保证数据的安全性 不会因为程序异常退出而导致数据丢失
    if (SaveGoodsInfo(head) != OPERATION_SUCCESSFUL) {
        printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
        return FAIL_TO_WRITE_FILE;
	}
    return OPERATION_SUCCESSFUL;
}

/**
 * @brief 添加商品
 * 
 * @param good 要添加的商品信息
 * @return int 
 * 
 * @note 返回值: 0 表示成功, -1 (TOO_MUCH_GOODS) 表示商品数量过多, -2 (REPEAT_GOOD) 表示商品已存在但信息不同, -3 (FAIL_TO_WRITE_FILE) 表示文件写入失败 (即商品信息保存失败)
 */
int AddGoods(Good good) {
    if (GoodsCount >= GOODS_MAX) {
        // printf("无法添加更多商品! 商品数量已达上限 100.\n");
        return TOO_MUCH_GOODS;
	}
    
	bool GoodExist = false;
	Node* node = head;
    while (node != NULL) {
        if (GoodsEqual(&node->value, &good)) {
            // 商品已存在 更新价格和剩余数量
            node->value.remaining += good.remaining; // 累加剩余数量
            GoodExist = true;
            break;
        }
		node = node->next;
    }

	// 商品不存在 则添加到链表中
    if (!GoodExist) {
        good.sign = GoodsCount + 1;
        Insert(&head, good);
    }
	// free(node);
    if (SaveGoodsInfo(head) != OPERATION_SUCCESSFUL) {
        // printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
        return FAIL_TO_WRITE_FILE;
    }
	return OPERATION_SUCCESSFUL;
}
    
/**
 * @brief 删除商品
 * 
 * @param name 要删除的商品名称
 * @return int 
 * 
 * @note 返回值: 0 表示成功, -3 (FAIL_TO_WRITE_FILE) 表示文件写入失败
 */
int DeleteGoods(const char* name){
	bool GoodFound = false;
    
	Node* node = head;
    while (node != NULL) {
        if (strcmp(node->value.name, name) == 0) {
			node->value.remaining = 0;
            Delete(head, node->value);
            GoodFound = true;
            break;
        }
		node = node->next;
    }

	if (SaveGoodsInfo(head) != OPERATION_SUCCESSFUL) {
        return FAIL_TO_WRITE_FILE;
	}

    // free(node);
    if (!GoodFound) {
        return NO_VALID_DATA;
	}
	return OPERATION_SUCCESSFUL;
}

void GoodsCopy(Good* dst, const Good src) {
    dst->sign = src.sign;
    strncpy(dst->name, src.name, sizeof(dst->name) - 1);
    dst->name[sizeof(dst->name) - 1] = '\0';
    dst->price = src.price;
    dst->remaining = src.remaining;
    strncpy(dst->factory, src.factory, sizeof(dst->factory) - 1);
    dst->factory[sizeof(dst->factory) - 1] = '\0';
    strncpy(dst->brand, src.brand, sizeof(dst->brand) - 1);
    dst->brand[sizeof(dst->brand) - 1] = '\0';
    dst->type = src.type;
}

bool GoodsEqual(const Good* a, const Good* b) {
    return strcmp(a->name, b->name) == 0 &&
        a->price == b->price &&
        strcmp(a->factory, b->factory) == 0 &&
        strcmp(a->brand, b->brand) == 0 &&
        a->type == b->type;
}