#define _CRT_SECURE_NO_WARNINGS

#include "identity.h"
#include "assist.h"
#include "UserOperation.h"
#include "AdminOperation.h"
#include "NodeList.h"
#include <windows.h>
#include <stdlib.h>

char* AccountName;
char* Password;
Good goods[100];
int GoodsCount = 0;

Good passGoods[100]; // 统计库存商品时 符合要求的商品会被存入 passGoods 数组 以便后续操作
int passCount = 0;
char str[4] = { 0 };

Node* head = NULL; // 商品链表的头指针

// 删除换行符 以便使用 fgets 函数
#define REMOVELINEFEED while (getchar() != '\n')

int Init() {
    /* --------- 申请内存 ---------*/
    AccountName = (char*) malloc(100 * sizeof(char));
    Password = (char*) malloc(100 * sizeof(char));

    /* ----------- 变量初始化 -----------*/
    GoodsCount = 0;
	memset(goods, 0, GOODS_MAX * sizeof(Good));
	memset(account, 0, 100 * sizeof(Account));

	if (AccountName == NULL || Password == NULL) {
        printf("内存分配失败!\n");
        return false;
	}
    
    memset(AccountName, 0, 100 * sizeof(char));
    memset(Password, 0, 100 * sizeof(char));
    GetAccountInfo();
    IsAdmin = false;
    return true;
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
    if (!Init()) {
        printf("内存申请失败! 请检查操作系统内存剩余! 系统即将退出...\n");
        return EXIT_FAILURE;
    }

    // <------------------- 登录 ------------------->
    int LoginRes = Login();
    if (LoginRes == UNEXISTED_ACCOUNT){ // 身份确定
        printf("登录失败! 请确定账号是否存在!\n");
        return EXIT_FAILURE;
    }
    else if (LoginRes == WRONG_PASSWORD) {
        printf("登录失败! 请确定密码是否正确!\n");
        return EXIT_FAILURE;
    }
    else {
        printf("登录成功! 欢迎回来!\n");
    }

    Sleep(2000);
    system("cls");

	// <------------------ 登录成功 ------------------>

    //IsAdmin = true; // 直接设置为管理员身份 以便测试管理员功能
    //strcpy(AccountName, "admin"); // 直接设置账号名 以便测试管理员功能
    
    // IsAdmin = false;
    // strcpy(AccountName, "User");

OPERATION:
    // <------------------ 获取输入 ------------------>
    printf("当前用户: %s\n", AccountName);
    ShowMenu(IsAdmin);
    int choice = 0;
    printf("请输入您的选择: ");
    choice = GetIntegerInput();
    if (choice == NOT_DIGIT) {
        printf("无效的输入! 请输入数字!\n");
		return EXIT_FAILURE;
	}

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
                printf("当前已有账户: \n");
                for (int i = 0; i < AccountCount; i++) {
                    printf("    %d. %s\n", i + 1, account[i].accountName);
				}

                printf("请输入账号名：");
				GetAccountName(TempAccount.accountName);
                
				// 不允许修改管理员账号的密码
                char LAccountName[100] = { 0 };
				memcpy(LAccountName, TempAccount.accountName, sizeof(TempAccount.accountName));
				toLower(LAccountName); // 将输入的账号名转换为小写字母 以便进行不区分大小写的比较

                // ADMIN_ACCOUNT_NAME 是纯小写字母
                if (strcmp(LAccountName, ADMIN_ACCOUNT_NAME) == 0) {
                    printf("不允许修改管理员账号!\n");
                    break;
				}

				bool VerifyPassed = false;

                for (int i = 0; i < AccountCount; i++) {
                    char storedName[100] = { 0 };
                    strcpy(storedName, account[i].accountName);
                    toLower(storedName);
                    
                    if (strcmp(storedName, LAccountName) == 0) {
                        VerifyPassed = true;
                        break;
                    }
                }

                if (VerifyPassed == false) {
                    printf("账号不存在!\n");
                    break;
				}

                char Confirm = '\0';
                printf("是否确定重置 %s 账号的密码? (y/n) ", TempAccount.accountName);
                Confirm = getchar();
                if (Confirm != 'Y' && Confirm != 'y') {
                    printf("重置 %s 账号的密码操作已取消!\n", TempAccount.accountName);
                    break;
                }

                printf("\n");
				// 验证通过 允许修改密码
                int res1 = ResetPassword(TempAccount);
                if (res1 == UNEXISTED_ACCOUNT) {
                    printf("账号不存在!\n");
                    break;
				}
                int res2 = SaveAccountInfo(); // 修改密码后保存账号信息至 account.csv
                if (res2 == FAIL_TO_OPEN_FILE) {
                    printf("文件打开失败!\n");
                    break;
                }
                else if (res2 == FAIL_TO_WRITE_FILE) {
                    printf("文件写入失败!\n");
                    break;
                }

                if (res1 == OPERATION_SUCCESSFUL && res2 == OPERATION_SUCCESSFUL) {
					printf("密码修改成功!\n");
                    break;
                }
                else {
					printf("密码修改失败!\n");
                    break;
                }
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
                int result = GetGoodsInfo(&head);
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
                    printf("Exit!\n");
                    break; // 退出修改商品信息
				}
                switch (choice) {
                    case 1:
                    {   
                        Good good = { 0 };
						printf("请输入商品名: \n");

                        while (getchar() != '\n');
                        fgets(good.name, 50, stdin);
						good.name[strcspn(good.name, "\n")] = '\0';

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
                        char name[50] = { 0 };
						printf("请输入要删除的商品名称: \n");
                        
                        while (getchar() != '\n');
                        fgets(name, 50, stdin);
						name[strcspn(name, "\n")] = '\0';
                        
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
                        GetGoodsInfo(&head);
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
                        // 按类型查询
                        if (choice == 1) {
                            bool Match = false;

                            printf("请输入要查询的商品类型: \n    1. 食品\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
							cond = GetIntegerInput();
                            if (cond < 1 || cond > 4 || cond == NOT_DIGIT) {
                                printf("无效的商品类型!\n");
                                break;
                            }

							Node* CurrentNode = head;
                            while (CurrentNode != NULL) {
                                if (CurrentNode->value.type == cond && CurrentNode->value.remaining > 0) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           CurrentNode->value.sign,
                                           CurrentNode->value.name,
                                           CurrentNode->value.price,
                                           CurrentNode->value.remaining,
                                           CurrentNode->value.factory,
                                           CurrentNode->value.brand,
                                           PrintGoodsType(CurrentNode->value.type)
                                    );
                                    Match = true;
                                }
                                CurrentNode = CurrentNode->next;
							}
                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
                            }

							// free(CurrentNode);
                        }
						// 按名称查询
                        else if (choice == 2) {
                            bool Match = false;

                            printf("请输入要查询的商品名称: \n");

							while (getchar() != '\n');
							fgets(condition, 100, stdin);
							condition[strcspn(condition, "\n")] = '\0';

                            Node* CurrentNode = head;
                            while (CurrentNode != NULL) {
                                if (CurrentNode->value.remaining == 0) {
                                    CurrentNode = CurrentNode->next;
                                    continue; // 跳过库存为0的商品
                                }
                                if (strcmp(CurrentNode->value.name, condition) == 0) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           CurrentNode->value.sign,
                                           CurrentNode->value.name,
                                           CurrentNode->value.price,
                                           CurrentNode->value.remaining,
                                           CurrentNode->value.factory,
                                           CurrentNode->value.brand,
                                           PrintGoodsType(CurrentNode->value.type)
                                    );
                                    Match = true;
                                }
                                CurrentNode = CurrentNode->next;

                            }
                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
                            }
							// free(CurrentNode);
                        }
                        else if (choice == 3) {
                            bool Match = false;

							printf("请输入要查询的生产厂家: \n");
                            
							while (getchar() != '\n');
							fgets(condition, 100, stdin);
							condition[strcspn(condition, "\n")] = '\0';

							Node* CurrentNode = head;
                            while (CurrentNode != NULL) {
                                if (CurrentNode->value.remaining == 0) {
                                    CurrentNode = CurrentNode->next;
                                    continue; // 跳过库存为0的商品
                                }
                                if (strcmp(CurrentNode->value.factory, condition) == 0) {
                                    printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                           CurrentNode->value.sign,
                                           CurrentNode->value.name,
                                           CurrentNode->value.price,
                                           CurrentNode->value.remaining,
                                           CurrentNode->value.factory,
                                           CurrentNode->value.brand,
                                           PrintGoodsType(CurrentNode->value.type)
                                    );
                                    Match = true;
                                }
                                CurrentNode = CurrentNode->next;
                            }

							// free(CurrentNode);
                            if (!Match) {
                                printf("没有找到符合条件的商品!\n");
							}
						}
						system("pause");
                        break;
                    }
                    case 4:
                    {
                        int type = 0;
						char name[100] = { 0 };

                        printf("请输入要修改的商品类型: \n    1. 食品\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
                        REMOVELINEFEED;
                        type = GetIntegerInput();
                        if (type < 1 || type > 5 || type == NOT_DIGIT) {
                            printf("无效的商品类型!\n");
                            break;
						}

                        bool Found = false;
						bool AppearedSign[100] = { false }; // 标记已经出现过的商品编号 避免重复显示同一编号的商品

                        Node* CurrentNode = head;
                        while (CurrentNode != NULL) {
                            if (CurrentNode->value.type == type && CurrentNode->value.remaining > 0) {
                                printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                       CurrentNode->value.sign,
                                       CurrentNode->value.name,
                                       CurrentNode->value.price,
                                       CurrentNode->value.remaining,
                                       CurrentNode->value.factory,
                                       CurrentNode->value.brand,
                                       PrintGoodsType(CurrentNode->value.type)
                                );

                                // 此处大概率不会出现下标越界的情况
								AppearedSign[CurrentNode->value.sign - 1] = true; // 标记该编号的商品已经出现过了
                                Found = true;
                            }
                            CurrentNode = CurrentNode->next;
                        }

                        if (!Found) {
                            printf("没有找到符合条件的商品!\n");
                            break;
                        }

                        int ModifiedSign = 0;
						printf("请输入要修改的商品编号: \n");
						ModifiedSign = GetIntegerInput();
                        if (ModifiedSign == NOT_DIGIT || ModifiedSign < 1) {
                            printf("无效的输入!\n");
                            break;
                        }
                        if (!AppearedSign[ModifiedSign - 1]) {
                             printf("无效的商品编号! 请从上面显示的商品中选择要修改的商品编号.\n");
                             break;
						}
                        
                        int choice = 0;
						CurrentNode = head;
                        while (CurrentNode != NULL) { // 哎呀没办法的 只能重新再遍历一次链表了
                            if (CurrentNode->value.sign == ModifiedSign) {
                                printf("请输入要修改的属性: \n    1. 名称\n    2. 价格\n    3. 生产厂家\n    4. 品牌\n");
                                choice = GetIntegerInput();
                                if (choice == NOT_DIGIT || choice < 1 || choice > 4) {
                                    printf("无效的输入! 请输入数字!\n");
                                    break;
                                }

                                char NewValue[100] = { 0 };
                                double NewPrice = 0;
                                if (choice == 1) {
                                    REMOVELINEFEED;
                                    fgets(NewValue, 100, stdin);
                                    NewValue[strcspn(NewValue, "\n")] = '\0';
                                    strcpy(CurrentNode->value.name, NewValue);
                                }
                                else if (choice == 2) {
                                    printf("请输入新的价格: \n");
                                    scanf("%lf", &NewPrice);
                                    if (CurrentNode->value.price == NewPrice) {
                                        printf("新价格与原价格相同! 无需修改!\n");
                                        break;
                                    }
                                    CurrentNode->value.price = NewPrice;
                                }
                                else if (choice == 3) {
                                    printf("请输入新的生产厂家: \n");
                                    scanf("%s", NewValue);
                                    strcpy(CurrentNode->value.factory, NewValue);
                                }
                                else { // choice == 4
                                    printf("请输入新的品牌: \n");
                                    scanf("%s", NewValue);
                                    strcpy(CurrentNode->value.brand, NewValue);
                                }
                                Found = true; // One more time
                            }

                            CurrentNode = CurrentNode->next;
						}

						int res = SaveGoodsInfo(head);
                        if (res == FAIL_TO_OPEN_FILE) {
                            printf("文件打开失败!\n");
                        }
                        else if (res == FAIL_TO_WRITE_FILE) {
                            printf("文件写入失败!\n");
						}
                        else if (res == OPERATION_SUCCESSFUL) {
							printf("商品信息修改成功!\n");
                        }
                        break;
					}
                    default:
                    {
                        printf("无效的选择!\n");
                        break;
                    }
                }
                break;
            }
            case 5:
            {
				GetGoodsInfo(&head); // 获取最新的商品信息以便统计时不覆盖之前的信息
                passCount = 0;
                memset(passGoods, 0, sizeof(passGoods));

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

                    Node* node = head;
                    while (node != NULL) {
                        if (node->value.remaining == 0) {
                            node = node->next;
                            continue;
                        }

                        if (node->value.price >= low && node->value.price <= high) {
                            passGoods[passCount].sign = node->value.sign;
                            passGoods[passCount].price = node->value.price;
                            strcpy(passGoods[passCount].name, node->value.name);
                            strcpy(passGoods[passCount].factory, node->value.factory);
                            passGoods[passCount].remaining = node->value.remaining;
                            strcpy(passGoods[passCount].brand, node->value.brand);
                            passGoods[passCount].type = node->value.type;
                            passCount++;
						}
						node = node->next;
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

                    // free(node);
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

                    Node* node = head;
                    while (node != NULL) {
                        if (node->value.remaining == 0) {
                            node = node->next;
                            continue;
                        }

                        if (node->value.remaining >= (unsigned int)low && node->value.remaining <= (unsigned int)high) {
                            passGoods[passCount].sign = node->value.sign;
                            passGoods[passCount].price = node->value.price;
                            strcpy(passGoods[passCount].name, node->value.name);
                            strcpy(passGoods[passCount].factory, node->value.factory);
                            passGoods[passCount].remaining = node->value.remaining;
                            strcpy(passGoods[passCount].brand, node->value.brand);
                            passGoods[passCount].type = node->value.type;
                            passCount++;
                        }
                        node = node->next;
                    }
                    // free(node);

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
					// 按照生产厂家统计时 先统计有哪些生产厂家 
                    // 然后让用户选择要查看的生产厂家 最后列出该生产厂家的所有商品
					char** FactoryList = (char**) malloc(GoodsCount * sizeof(char*)); // 生产厂家列表
                    int FactoryCount = 0; // 生产厂家数量
                    Node* node = head;
                    while (node != NULL) {
                        if (node->value.remaining == 0) {
                            node = node->next;
                            continue;
                        }
                        bool FactoryExist = false;
                        for (int i = 0; i < FactoryCount; i++) {
                            if (strcmp(FactoryList[i], node->value.factory) == 0) {
                                FactoryExist = true;
                                break;
                            }
                        }
                        if (!FactoryExist) {
                            FactoryList[FactoryCount] = (char*) malloc(100 * sizeof(char));
                            strcpy(FactoryList[FactoryCount], node->value.factory);
                            FactoryCount++;
                        }
                        node = node->next;
                    }
                    if (FactoryCount == 0) {
                        printf("没有找到符合条件的商品!\n");
                    }
                    else {
                        printf("共有 %d 个生产厂家: \n", FactoryCount);
                        for (int i = 0; i < FactoryCount; i++) {
                            printf("    %d. %s\n", i + 1, FactoryList[i]);
						}
                        printf("请输入要查看的生产厂家编号: \n");
                        int sign = GetIntegerInput();
                        if (sign == NOT_DIGIT || sign < 1 || sign > FactoryCount) {
                            printf("无效的输入!\n");
                            break;
                        }
						printf("生产厂家 '%s' 的商品有: \n", FactoryList[sign - 1]);
						Node* CurrentNode = head;
                        while (CurrentNode != NULL) {
                            if (CurrentNode->value.remaining == 0) {
                                CurrentNode = CurrentNode->next;
                                continue; // 跳过库存为0的商品
                            }
                            if (strcmp(CurrentNode->value.factory, FactoryList[sign - 1]) == 0) {
                                printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                       CurrentNode->value.sign,
                                       CurrentNode->value.name,
                                       CurrentNode->value.price,
                                       CurrentNode->value.remaining,
                                       CurrentNode->value.factory,
                                       CurrentNode->value.brand,
                                       PrintGoodsType(CurrentNode->value.type)
                                );
                            }
                            CurrentNode = CurrentNode->next;
                        }
						// free(CurrentNode);
                    }

                    // free memory
                    for (int i = 0; i < FactoryCount; i++) {
                        free(FactoryList[i]);
                    }
					free(FactoryList);
                    // free(node);
                }
                system("pause");
                break;
            }
            case 6:
            {
				GetGoodsInfo(&head); // 获取最新的商品信息以便保存时不覆盖之前的信息
                if (SaveGoodsInfo(head) != OPERATION_SUCCESSFUL) {
                    printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
                }
                else {
					printf("保存商品信息成功!\n");
                }
                break;
            }
            case 7:
            {
                int result = GetGoodsInfo(&head);
                if (result == OPERATION_SUCCESSFUL) {
					printf("读取商品信息成功!\n");
                    Print(head);
                }
                else if (result == FAIL_TO_OPEN_FILE) {
                    printf("打开文件失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
                }
                else if (result == TOO_MUCH_GOODS) {
                    printf("商品数量过多, 文件中只读取了前100个商品. 当前数量: %d\n", GoodsCount);
                }
                else if (result == NO_VALID_DATA) {
                    printf("文件中没有有效的商品数据!\n");
                }
                system("pause");
                break;
            }
            case 8:
            {
                printf("Exit!\n");
                return EXIT_SUCCESS;
            }
            default:
            {
                printf("无效的选择!\n");
                break;
            }
        }
        Sleep(2000);
		system("cls");
        goto OPERATION;
    }
    else {
        switch (choice) {
            case 1:
            {
                // change account info
				Account TempAccount;
				memset(&TempAccount, 0, sizeof(Account));
				strcpy(TempAccount.accountName, AccountName);
				printf("请输入旧密码：");
				GetAccountPassword(TempAccount.password);
				printf("\n");
				char newPassword[100] = { 0 };
				printf("请输入新密码：");
				GetAccountPassword(newPassword);
                printf("\n");

				int result = ChangeAccountPassword(TempAccount, newPassword);
                if (result == OPERATION_SUCCESSFUL) {
                    printf("密码修改成功!\n");
                }
                else if (result == WRONG_PASSWORD) {
                    printf("原密码错误! 修改密码失败!\n");
                }
                else if (result == FAIL_TO_OPEN_FILE) {
                    printf("文件打开失败! 请检查是否具有读取文件的权限或文件是否存在.\n");
                }
                else if (result == FAIL_TO_WRITE_FILE) {
                    printf("文件写入失败! 请检查是否具有写入文件的权限.\n");
				}
                else if (result == UNEXISTED_ACCOUNT) {
                    printf("账号不存在! 修改密码失败!\n");
				}
                else if (result == NOT_ALLOWED_OPERATION) {
					printf("不允许的操作! 修改密码失败!\n");
                }
                else if (result == SAME_PASSWORD) {
                    printf("新密码与旧密码相同!\n");
				}
                break;
            }
            case 2:
            {
				GetGoodsInfo(&head);
                // COPY FROM ADMIN OPERATION
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

                    Node* node = head;
                    while (node != NULL) {
                        if (node->value.type == cond && node->value.remaining > 0) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   node->value.sign,
                                   node->value.name,
                                   node->value.price,
                                   node->value.remaining,
                                   node->value.factory,
                                   node->value.brand,
                                   PrintGoodsType(node->value.type)
                            );
                            Match = true;
                        }
                        node = node->next;
					}
                    // free(node);

                    if (!Match) {
                        printf("没有找到符合条件的商品!\n");
                    }
                }
                // 按名称查询
                else if (choice == 2) {
                    printf("请输入要查询的商品名称: \n");

                    REMOVELINEFEED;
					fgets(condition, 100, stdin);
					condition[strcspn(condition, "\n")] = '\0';
                    
					Node* node = head;
                    while (node != NULL) {
                        if (node->value.remaining == 0) {
                            node = node->next;
                            continue; // 跳过库存为0的商品
                        }
                        // 忽略大小写
                        // L prefix -> lowercase
                        char* LCondition = malloc(100 * sizeof(char));
                        if (LCondition == NULL) {
                            printf("内存分配失败!\n");
                            break;
						}
                        strcpy(LCondition, condition);
                        toLower(LCondition);

                        char* LName = malloc(100 * sizeof(char));
                        if (LName == NULL) {
                            printf("内存分配失败!\n");
                            free(LCondition);
                            break;
                        }
                        strcpy(LName, node->value.name);
                        toLower(LName);

                        if (strcmp(LName, LCondition) == 0) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   node->value.sign,
                                   node->value.name,
                                   node->value.price,
                                   node->value.remaining,
                                   node->value.factory,
                                   node->value.brand,
                                   PrintGoodsType(node->value.type)
                            );
                            Match = true;
                        }
                        free(LCondition);
                        free(LName);
                        node = node->next;
					}

                    if (!Match) {
                        printf("没有找到符合条件的商品!\n");
                    }
                }
                else if (choice == 3) {
                    printf("请输入要查询的生产厂家: \n");
                    
					REMOVELINEFEED;
					fgets(condition, 100, stdin);
					condition[strcspn(condition, "\n")] = '\0';
                    
					Node* node = head;
                    while (node != NULL) {
                        if (node->value.remaining == 0) {
                            node = node->next;
							continue; // 跳过库存为0的商品
                        }

                        if (strcmp(condition, node->value.factory) == 0) {
                            printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                                   node->value.sign,
                                   node->value.name,
                                   node->value.price,
                                   node->value.remaining,
                                   node->value.factory,
                                   node->value.brand,
                                   PrintGoodsType(node->value.type)
                            );
                            Match = true;
                        };
						node = node->next;
                    }
                    // free(node);

                    if (!Match) {
                        printf("没有找到符合条件的商品!\n");
                    }
                }
				system("pause");
                break;
            }
            case 3:
            {
				char* GoodsName = (char*) malloc(100 * sizeof(char));
				printf("请输入要购买的商品类型: \n    1. 食物\n    2. 化妆品\n    3. 日用品\n    4. 饮料\n");
                int GoodsType = 0;
				GoodsType = GetIntegerInput();
                if (GoodsType < 1 || GoodsType > 4 || GoodsType == NOT_DIGIT) {
                    printf("无效的商品类型!\n");
                    free(GoodsName);
                    break;
                }
				GetGoodsInfo(&head); // 获取最新的商品信息以便购买时不覆盖之前的信息

				bool HasGoods = false; // 判断当前商品是否有符合条件的
                Node* node = head;
                while (node != NULL) {
                    if (node->value.type == GoodsType && node->value.remaining > 0) {
                        printf("编号: %d, 名称: %s, 价格: %.2f, 剩余: %u, 厂家: %s, 品牌: %s, 类型: %s\n",
                               node->value.sign,
                               node->value.name,
                               node->value.price,
                               node->value.remaining,
                               node->value.factory,
                               node->value.brand,
                               PrintGoodsType(node->value.type)
                        );
                        HasGoods = true;
                    }
                    node = node->next;
                }

                if (!HasGoods) {
                    printf("没有找到符合条件的商品!\n");
                    free(GoodsName);
                    break;
                }

                printf("请输入要购买商品的序号: \n");
                int sign = 0;
				sign = GetIntegerInput();
                if (sign == NOT_DIGIT || sign < 1 || sign > GoodsCount) {
                    printf("无效的输入!\n");
                    free(GoodsName);
                    break;
				}

                Node* CurrentNode = head;
                while (CurrentNode != NULL) {
                    if (CurrentNode->value.sign == sign) {
                        if (CurrentNode->value.type != GoodsType) {
                            printf("编号与商品类型不匹配!\n");
                            free(GoodsName);
                            break;
                        }
                        strcpy(GoodsName, CurrentNode->value.name);
                        break;
                    }
                    CurrentNode = CurrentNode->next;
				}
                int amount = 0;
                printf("请输入要购买的数量: \n");
				amount = GetIntegerInput();
                if (amount == NOT_DIGIT || amount <= 0) {
                    printf("无效的输入! 请输入数字!\n");
                    free(GoodsName);
                    break;
				}

                int result = BuyGoods(GoodsType, GoodsName, amount);
                switch (result) {
                    case OPERATION_SUCCESSFUL:
                        printf("购买成功!\n");
                        break;
                    case NOT_ENOUGH_GOODS:
                        printf("库存不足!\n");
                        break;
                    case NO_VALID_DATA:
                        printf("没有找到符合条件的商品!\n");
                        break;
                    case FAIL_TO_WRITE_FILE:
                        printf("写入文件失败!\n");
                        break;
                    default:
                        printf("未知错误!\n");
                        break;
                }

                free(GoodsName);
                break;
            }
            case 4:
            {
                printf("Exit!\n");
                return EXIT_SUCCESS;
            }
            default:
            {
                printf("无效的选择!\n");
                break;
            }
        }
        Sleep(2000);
		system("cls");
        goto OPERATION;
    }

    Clear(&head);
	free(AccountName);
	free(Password);

    return EXIT_SUCCESS;
}