#define _CRT_SECURE_NO_WARNINGS

#include "identity.h"
#include "assist.h"
#include "UserOperation.h"
#include "AdminOperation.h"
#include <stdlib.h>
#include <windows.h>

char* AccountName;
char* Password;
Good goods[100];
int GoodsCount = 0;

Good passGoods[100]; // 统计库存商品时 符合要求的商品会被存入 passGoods 数组 以便后续操作
int passCount = 0;
char str[4] = { 0 };

void Init() {
    /* --------- 申请内存 ---------*/
    AccountName = (char*) malloc(100 * sizeof(char));
    Password = (char*) malloc(100 * sizeof(char));

    /* ----------- 变量初始化 -----------*/
    GoodsCount = 0;
	memset(goods, 0, GOODS_MAX * sizeof(Good));
	memset(account, 0, 100 * sizeof(Account));
    memset(AccountName, 0, 100 * sizeof(char));
    memset(Password, 0, 100 * sizeof(char));
    GetAccountInfo();
    IsAdmin = false;
}

int Login() {
	printf("请输入账号：");
    GetAccountName(AccountName);
	printf("请输入密码：");
    GetAccountPassword(Password);
    printf("\n");

    char TempAccountName[100] = { 0 };
    strcpy(TempAccountName, AccountName); // 备份账号名 因为 toLower 会修改它
    toLower(TempAccountName); // 将账号名转换为小写字母 以便进行不区分大小写的比较

    // 如果账号名是管理员账号 则继续验证密码 否则直接确认普通用户身份
    if (strcmp(TempAccountName, ADMIN_ACCOUNT_NAME) == 0) {
        if (strcmp(Password, ADMIN_PASSWORD) == 0) {
            IsAdmin = true;
			return LOGIN_SUCCESSFULLY;
        }
        else { // 是管理员账号 但是密码错误
			return WRONG_PASSWORD;
        }
    }
    else { // 不是管理员账号
        bool AccountFound = false;
        for (int i = 0; i < AccountCount; i++) {
            if (strcmp(TempAccountName, account[i].accountName) == 0) {
                IsAdmin = account[i].isAdmin;
                AccountFound = true;
                if (strcmp(Password, account[i].password) != 0) {
					return WRONG_PASSWORD; // 账号存在但是密码错误
                }
            }
        }
        if (AccountFound) {
			return LOGIN_SUCCESSFULLY;
        }
        else {
			return UNEXISTED_ACCOUNT;
        }
    }
}

int main() {
    Init();

    // <------------------- 登录 ------------------->
 //   int LoginRes = Login();
 //   if (LoginRes == UNEXISTED_ACCOUNT){ // 身份确定
 //       printf("登录失败! 请确定账号是否存在!\n");
 //       return EXIT_FAILURE;
 //   }
 //   else if (LoginRes == WRONG_PASSWORD) {
 //       printf("登录失败! 请确定密码是否正确!\n");
 //       return EXIT_FAILURE;
	//}
 //   else {
 //       printf("登录成功! 欢迎回来!\n");
 //   }

 //   Sleep(2000);
 //   system("cls");

	// <------------------ 登录成功 ------------------>

    IsAdmin = true; // 直接设置为管理员身份 以便测试管理员功能
    memcpy(AccountName, "admin", 6); // 直接设置账号名 以便测试管理员功能
    
    // <------------------ 获取输入 ------------------>
    printf("当前用户: %s\n", AccountName);
    ShowMenu(IsAdmin);
    int choice = 0;
    printf("请输入您的选择: ");
    char temp = getchar();
    if (!isdigit(temp)) {
        printf("无效的输入! 请输入数字!\n");
        return EXIT_FAILURE;
	}
	choice = temp - '0'; // 将字符数字转换为整数

    while (getchar() != '\n'); // 清除输入缓冲区中的换行符 因为 GetAccountName 用的是 fgets 函数 如果不加的话会导致 accountName 为换行符

    if (IsAdmin) {
        switch (choice) {
            case 1:
            {
                Account NewAccount;
                memset(&NewAccount, 0, sizeof(Account));

                printf("请输入账号：");
				GetAccountName(NewAccount.accountName); // 获取新账号名
				if (strcmp(ADMIN_ACCOUNT_NAME, NewAccount.accountName) == 0) { // 输入的账号名是管理员账号
                    printf("不允许修改管理员账号!\n");
                    break;
                }
                
				// 检查账号是否已存在
                bool AccountExist = false;
                for (int i = 0; i < AccountCount; i++) {
                    if (strcmp(account[i].accountName, NewAccount.accountName) == 0) {
                        printf("账号已存在!\n");
                        AccountExist = true;
                        break;
                    }
				}
                if (AccountExist) {
					// 账号已存在 重新输入
					// 将已有数据清空 以便重新输入
					memset(NewAccount.accountName, 0, 100 * sizeof(char));
					memset(NewAccount.password, 0, 100 * sizeof(char));
                    break;
                }

				printf("请输入新账户密码：");
                GetAccountPassword(NewAccount.password);
                printf("\n");

                int res = AddAccount(NewAccount);

                if (res == OPERATION_SUCCESSFUL) {
                    printf("%s 账户添加成功!\n", NewAccount.accountName);
                }
                else if (res == FAIL_TO_OPEN_FILE) {
                    printf("文件打开失败!\n");
                }
                else if (res == FAIL_TO_WRITE_FILE) {
                    printf("文件写入失败!\n");
                }

                break;
            }
            case 2:
            {
				Account TempAccount;
				memset(&TempAccount, 0, sizeof(Account));

                printf("请输入账号名：");
				GetAccountName(TempAccount.accountName);
                
				// 不允许修改管理员账号的密码
                char t[100] = { 0 };
				memcpy(t, TempAccount.accountName, sizeof(TempAccount.accountName));
				toLower(t); // 将输入的账号名转换为小写字母 以便进行不区分大小写的比较
                if (strcmp(t, ADMIN_ACCOUNT_NAME) == 0) {
                    printf("不允许修改管理员账号!\n");
                    break;
				}

                printf("请输入旧密码：");
				GetAccountPassword(TempAccount.password);
                printf("\n");

				bool VerifyPassed = false;
                for (int i = 0; i < AccountCount; i++) {
                    if (strcmp(account[i].accountName, TempAccount.accountName) == 0) { // 账号存在
                        if (strcmp(account[i].password, TempAccount.password) != 0) {
                            // 密码错误
                            printf("密码错误!\n");
                            return EXIT_FAILURE;
                        }
                        memcpy(account[i].password, TempAccount.password, sizeof(TempAccount.password));
                        VerifyPassed = true;
                        break;
                    }
                }

                if (!VerifyPassed) {
                    printf("账号不存在!\n");
                    break;
				}

				// 验证通过 允许修改密码
                ResetPassword(TempAccount);
                SaveAccountInfo(); // 修改密码后保存账号信息至 account.csv
                break;
            }
            case 3:
            {
                int result = SellGoods();
                if (result == FAIL_TO_OPEN_FILE) {
                    printf("打开文件失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
                }
                else if (result == TOO_MUCH_GOODS) {
                    printf("商品数量过多，文件中只读取了前100个商品. 当前数量: %d\n", GoodsCount);
                }
                else if (result == NO_VALID_DATA) {
                    printf("文件中没有有效的商品数据!\n");
				}
                // else
				// 这里不再处理剩下的情况 因为 SellGoods 函数内部已经处理了剩余的情况 
                // 包括打印售卖结果和更新商品信息文件等操作
                break;
            }
            case 4:
            {
                int result = GetGoodsInfo();
                if (result == FAIL_TO_OPEN_FILE) {
                     printf("打开文件失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
					 return EXIT_FAILURE;
                }
                else if (result == TOO_MUCH_GOODS) {
                     printf("商品数量过多，文件中只读取了前100个商品. 当前数量: %d\n", GoodsCount);
                     return EXIT_FAILURE;
                }
                else if (result == NO_VALID_DATA) {
                     printf("文件中没有有效的商品数据!\n");
                     return EXIT_FAILURE;
                }
				printf("请选择修改方式: \n    1. 增加商品\n    2. 删除商品\n    3. 查询商品信息\n    4. 修改商品信息\n    5. 退出\n");
                int choice = GetIntegerInput();
                if (choice == NOT_DIGIT || choice < 1 || choice > 5) {
                    printf("无效的输入! 请输入数字!\n");
					break;
                }
                if (choice == 5) {
                    break; // 退出修改商品信息
				}
                switch (choice) {
                    case 1:
                    {   
                        Good good = { 0 };
						printf("请输入商品名: \n");
						scanf("%s", good.name);
						printf("请输入商品价格: \n");
						scanf("%lf", &good.price);
						printf("请输入商品剩余数量: \n");
						scanf("%u", &good.remaining);
                        if (good.remaining <= 0) {
                            printf("商品剩余数量不合法!\n");
                            break;
						}
                        printf("请输入商品生产厂家: \n");
						scanf("%s", good.factory);
						printf("请输入商品品牌: \n");
						scanf("%s", good.brand);
						printf("请输入商品类型: \n    1. 食品\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
						scanf("%s", str);
						int type = str2int(str);
                        if (type < 1 || type > 4 || type == NOT_DIGIT) {
                            printf("无效的商品类型!\n");
                            break;
                        }
						good.type = type;
                        int res = AddGoods(good);
                        switch (res) {
                            case OPERATION_SUCCESSFUL:
                                printf("商品添加成功!\n");
                                break;
                            case TOO_MUCH_GOODS:
                                printf("无法添加更多商品! 商品数量已达上限 100.\n");
                                break;
                            case REPEAT_GOOD:
                                printf("商品 '%s' 已存在但信息不同! 请检查输入的商品信息是否正确.\n", good.name);
                                break;
                            case FAIL_TO_OPEN_FILE:
                                printf("文件打开失败!\n");
								break;
                            default:
                                break;
                        }
                        break;
                    }
                    case 2:
                    {
                        char name[100] = { 0 };
						printf("请输入要删除的商品名称: \n");
						scanf("%s", name);
                        int res = DeleteGoods(name);
                        if (res == OPERATION_SUCCESSFUL) {
                            printf("商品删除成功!\n");
						}
                        else if (res == NO_VALID_DATA) {
                            printf("商品不存在!\n");
                        }
                        break;
                    }
                    case 3:
                    {
                        // 因为查询商品时需要提供的参数类型不固定 这里直接写在 main 里面了
						printf("请输入查询条件: \n    1. 按类型查询\n    2. 按名称查询\n    3. 按生产厂家查询\n");
                        int choice = 0;
						int cond = 0; // condition 要查询的信息
                        char condition[100] = { 0 };
                        choice = GetIntegerInput();
                        if (choice == NOT_DIGIT) {
                            printf("无效的输入! 请输入数字!\n");
                        }
                        if (choice < 1 || choice > 3) {
                            printf("无效的选择!\n");
                            break;
						}
						bool Match = false;
                        // 按类型查询
                        if (choice == 1) {
                            printf("请输入要查询的商品类型: \n    1. 食品\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
							cond = GetIntegerInput();
                            if (cond < 1 || cond > 4 || cond == NOT_DIGIT) {
                                printf("无效的商品类型!\n");
                                break;
                            }

                            for (int i = 0; i < GoodsCount; i++) {
                                if (goods[i].remaining == 0) {
                                    continue; // 跳过库存为0的商品
                                }
                                if (goods[i].type == cond) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           goods[i].sign,
                                           goods[i].name,
                                           goods[i].price,
                                           goods[i].remaining,
                                           goods[i].factory,
                                           goods[i].brand,
                                           PrintGoodsType(goods[i].type)
                                    );
                                    Match = true;
                                }
							}
                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
                            }
                        }
						// 按名称查询
                        else if (choice == 2) {
                            printf("请输入要查询的商品名称: \n");
							scanf("%s", condition);
                            for (int i = 0; i < GoodsCount; i++) {
                                if (goods[i].remaining == 0) {
                                    continue; // 跳过库存为0的商品
                                }
                                if (strcmp(goods[i].name, condition) == 0) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           goods[i].sign,
                                           goods[i].name,
                                           goods[i].price,
                                           goods[i].remaining,
                                           goods[i].factory,
                                           goods[i].brand,
                                           PrintGoodsType(goods[i].type)
                                    );
                                    Match = true;
                                }
                            }

                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
							}
                        }
                        else if (choice == 3) {
							printf("请输入要查询的生产厂家: \n");
                            scanf("%s", condition);
                            for (int i = 0; i < GoodsCount; i++) {
                                if (goods[i].remaining == 0) {
                                    continue; // 跳过库存为0的商品
                                }
                                if (strcmp(goods[i].factory, condition) == 0) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           goods[i].sign,
                                           goods[i].name,
                                           goods[i].price,
                                           goods[i].remaining,
                                           goods[i].factory,
                                           goods[i].brand,
                                           PrintGoodsType(goods[i].type)
                                    );
                                    Match = true;
                                }
							}

                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
							}
						}
                        break;
                    }
                    case 4:
                    {
                        int type = 0;
						char name[100] = { 0 };
						printf("请输入要修改的商品类型: \n    1. 食品\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
						type = GetIntegerInput();
                        if (type < 1 || type > 5 || type == NOT_DIGIT) {
                            printf("无效的商品类型!\n");
                            break;
						}

                        bool Found = false;
						printf("请输入要修改的商品名称: \n");
						scanf("%s", name);

                        for (int i = 0; i < GoodsCount; i++) {
                            if (goods[i].remaining == 0) {
                                continue; // 跳过库存为0的商品
                            }
                            if (goods[i].type == type && strcmp(goods[i].name, name) == 0) {
								printf("请输入要修改的属性: \n    1. 名称\n    2. 价格\n    3. 生产厂家\n    4. 品牌\n");
								choice = GetIntegerInput();
                                if (choice == NOT_DIGIT || choice < 1 || choice > 4) {
                                    printf("无效的输入! 请输入数字!\n");
                                    break;
								}
								Found = true;
								char newValue[100] = { 0 };
                                double value = 0;
                                if (choice == 1) {
									printf("请输入新的名称: \n");
                                    scanf("%s", newValue);
									strcpy(goods[i].name, newValue);
                                }
                                else if (choice == 2) {
                                    printf("请输入新的价格: \n");
									scanf("%lf", &value);
									goods[i].price = value;
                                }
                                else if (choice == 3) {
                                    printf("请输入新的生产厂家: \n");
									scanf("%s", newValue);
									strcpy(goods[i].factory, newValue);
                                }
                                else if (choice == 4) {
                                    printf("请输入新的品牌: \n");
									scanf("%s", newValue);
									strcpy(goods[i].brand, newValue);
								}
                                SaveGoodsInfo(); // 修改商品信息后保存至 goodsinfo.csv
                            }
                        }
                        if (!Found) {
                            printf("未找到符合条件的商品!\n");
                        }
					}
                    default:
                    {
                        printf("无效的选择!\n");
                        break;
                    }
                }
            }
            case 5:
            {
				GetGoodsInfo(); // 获取最新的商品信息以便统计时不覆盖之前的信息
                int choice = 0;
				printf("请选择统计方式: \n    1. 按价格统计\n    2. 按库存量统计\n    3. 按生产厂家统计\n");
				choice = GetIntegerInput();
                if (choice == NOT_DIGIT || choice < 1 || choice > 3) {
                    printf("无效的输入! 请输入数字!\n");
                    break;
                }

                if (choice == 1) {
					printf("请输入价格上限: ");
                    double high = 0, low = 0;
					high = GetDoubleInput();
                    if (high == NOT_DIGIT || high <= 0) {
                        printf("无效的输入!\n");
                        break;
					}
					printf("请输入价格下限: ");
					low = GetDoubleInput();
                    if (low == NOT_DIGIT || low < 0 || low > high) {
                        printf("无效的输入!\n");
                        break;
					}

                    for (int i = 0; i < GoodsCount; i++) {
                        if (goods[i].remaining == 0) {
                            continue; // 跳过库存为0的商品
                        }
                        if (goods[i].price >= low && goods[i].price <= high) {
							passGoods[passCount].sign = goods[i].sign;
                            passGoods[passCount].price = goods[i].price;
                            strcpy(passGoods[passCount].name, goods[i].name);
                            strcpy(passGoods[passCount].factory, goods[i].factory);
                            passGoods[passCount].remaining = goods[i].remaining;
                            strcpy(passGoods[passCount].brand, goods[i].brand);
                            passGoods[passCount].type = goods[i].type;
                            passCount++;
                        }
                    }
                    if (passCount == 0) {
                        printf("没有找到符合条件的商品!\n");
					}
                    else {
                        // 按照价格排序
                        if (passCount > 1) {
                            int sort = 0;
                            printf("升序还是降序? \n    1. 升序\n    2. 降序\n");
                            sort = GetIntegerInput();
                            if (sort == NOT_DIGIT || sort < 1 || sort > 2) {
                                printf("无效的输入! 请输入数字!\n");
                                break;
                            }
                            if (sort == 1) {
                                qsort(passGoods, passCount, sizeof(Good), PriceAscend);
                            }
                            else {
                                qsort(passGoods, passCount, sizeof(Good), PriceDescend);
                            }
                        }

                        printf("价格在 %.2lf 到 %.2lf 之间的商品有: \n", low, high);
                        for (int i = 0; i < passCount; i++) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   passGoods[i].sign,
                                   passGoods[i].name,
                                   passGoods[i].price,
                                   passGoods[i].remaining,
                                   passGoods[i].factory,
                                   passGoods[i].brand,
                                   PrintGoodsType(passGoods[i].type)
							);
                        }
                    }
                }
                else if (choice == 2){
					printf("请输入库存量上限: ");
					int high = 0, low = 0;
                    high = GetIntegerInput();
                    if (high == NOT_DIGIT || high <= 0) {
                        printf("无效的输入!\n");
                        break;
                    }
					printf("请输入库存量下限: ");
                    low = GetIntegerInput();
                    if (low == NOT_DIGIT || low < 0 || low > high) {
                        printf("无效的输入!\n");
                        break;
                    }

                    for (int i = 0; i < GoodsCount; i++) {
                        if (goods[i].remaining == 0) {
                            continue; // 跳过库存为0的商品
                        }
						if (goods[i].remaining >= low && goods[i].remaining <= high) {
                            passGoods[passCount].sign = goods[i].sign;
                            passGoods[passCount].price = goods[i].price;
                            strcpy(passGoods[passCount].name, goods[i].name);
                            strcpy(passGoods[passCount].factory, goods[i].factory);
                            passGoods[passCount].remaining = goods[i].remaining;
                            strcpy(passGoods[passCount].brand, goods[i].brand);
                            passGoods[passCount].type = goods[i].type;
                            passCount++;
                        }
                    }
                    if (passCount == 0) {
                        printf("没有找到符合条件的商品!\n");
                    }
                    else {
                        if (passCount > 1) {
                            int sort = 0;
                            printf("升序还是降序? \n    1. 升序\n    2. 降序\n");
                            sort = GetIntegerInput();
                            if (sort == NOT_DIGIT || sort < 1 || sort > 2) {
                                printf("无效的输入! 请输入数字!\n");
                                break;
                            }

                            printf("库存量在 %d 到 %d 之间的商品有: \n", low, high);
                            if (sort == 1)
                                qsort(passGoods, passCount, sizeof(Good), RemainingAscend);
                            else
                                qsort(passGoods, passCount, sizeof(Good), RemainingDescend);
                        }

                        for (int i = 0; i < passCount; i++) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   passGoods[i].sign,
                                   passGoods[i].name,
                                   passGoods[i].price,
                                   passGoods[i].remaining,
                                   passGoods[i].factory,
                                   passGoods[i].brand,
								   PrintGoodsType(passGoods[i].type)
                                   );
                        }
                    }
                }
				else if (choice == 3) {
					// 按照生产厂家统计时 先统计有哪些生产厂家 然后让用户选择要查看的生产厂家 最后列出该生产厂家的所有商品
                    int Factory[100] = { 0 }; // 存储生产厂家在 goods 数组中的索引
					int factoryCount = 0; // 生产厂家数量
                    bool exists = false;
                    for (int i = 0; i < GoodsCount; i++) {
                        for (int j = 0; j < factoryCount; j++) {
                            if (strcmp(goods[i].factory, goods[Factory[j]].factory) == 0) {
                                exists = true;
                                break;
                            }
                        }
                        if (!exists) {
                            Factory[factoryCount++] = i;
                        }
                        exists = false;
                    }

					printf("共有 %d 个生产厂家: \n", factoryCount);
                    for (int i = 0; i < factoryCount; i++) {
                        printf("    %d. %s\n", i + 1, goods[ Factory[i] ].factory);
					}
					printf("请输入要查看的生产厂家编号: \n");
					int sign = GetIntegerInput();
                    if (sign == NOT_DIGIT || sign < 1 || sign > factoryCount) {
                        printf("无效的输入! 请输入数字!\n");
                        break;
					}
                    printf("生产厂家 '%s' 的商品有: \n", goods[ Factory[sign - 1] ].factory);
                    for (int i = 0; i < GoodsCount; i++) {
                        if (goods[i].remaining == 0) {
                            continue; // 跳过库存为0的商品
                        }
                        if (strcmp(goods[i].factory, goods[Factory[sign - 1]].factory) == 0) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   goods[i].sign,
                                   goods[i].name,
                                   goods[i].price,
                                   goods[i].remaining,
                                   goods[i].factory,
                                   goods[i].brand,
                                   PrintGoodsType(goods[i].type)
                            );
                        }
                    }
                }
                break;
            }
            case 6:
            {
				GetGoodsInfo(); // 获取最新的商品信息以便保存时不覆盖之前的信息
                if (SaveGoodsInfo() != OPERATION_SUCCESSFUL) {
                    printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
                }
                else {
					printf("保存商品信息成功!\n");
                }
                break;
            }
            case 7:
            {
                int result = GetGoodsInfo();
                if (result == OPERATION_SUCCESSFUL) {
					printf("读取商品信息成功!\n");
                    for (int i = 0; i < GoodsCount; i++) {
                        if (goods[i].remaining == 0) {
                            continue; // 跳过库存为0的商品
                        }
                        printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                               goods[i].sign,
                               goods[i].name,
                               goods[i].price,
                               goods[i].remaining,
                               goods[i].factory,
                               goods[i].brand,
                               PrintGoodsType(goods[i].type)
                        );
					}
                }
                else if (result == FAIL_TO_OPEN_FILE) {
                    printf("打开文件失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
                }
                else if (result == TOO_MUCH_GOODS) {
                    printf("商品数量过多，文件中只读取了前100个商品. 当前数量: %d\n", GoodsCount);
                }
                else if (result == NO_VALID_DATA) {
                    printf("文件中没有有效的商品数据!\n");
                }
                break;
            }
            case 8:
            {
                printf("Exit!\n");
                break;
            }
            default:
            {
                printf("无效的选择!\n");
                break;
            }
        }
    }
    else {
        switch (choice) {
            case 1:
            {
                ChangeAccountInfo();
                break;
            }
            case 2:
            {
                QueryInfo();
                break;
            }
            case 3:
            {
                BuyGoods();
                break;
            }
            default:
            {
                printf("无效的选择!\n");
                break;
            }
        }
    }

	free(AccountName);
	free(Password);

    return EXIT_SUCCESS;
}