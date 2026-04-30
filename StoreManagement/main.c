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

void Init() {
    /* --------- 申请内存 ---------*/
    AccountName = (char*) malloc(100 * sizeof(char));
    Password = (char*) malloc(100 * sizeof(char));

    /* ----------- 变量初始化 -----------*/
    GoodsCount = 0;
	memset(goods, 0, 100 * sizeof(Good));
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
			return UNEXISTED_ACCOUNT;
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

    // <------------------ 登录 ------------------->
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

	// <------------------ 登录成功 ------------------->

    //IsAdmin = true; // 直接设置为管理员身份 以便测试管理员功能
    //memcpy(AccountName, "admin", 6); // 直接设置账号名 以便测试管理员功能

    Sleep(2000);
    system("cls");
    
    // <----------------- 获取输入 ------------------>
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

                if (res == 0) {
                    printf("%s账户添加成功!\n", NewAccount.accountName);
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
				// 剩下的输出内容在 SellGoods 函数中已经处理了 这里不需要再处理了
                break;
            }
            case 4:
            {
                ManageGoods();
                break;
            }
            case 5:
            {
                InventoryStatistics();
                break;
            }
            case 6:
            {
                if (SaveGoodsInfo() != 0) {
                    printf("保存商品信息失败! 请检查是否具有写入文件的权限.\n");
                }
                break;
            }
            case 7:
            {
                int result = GetGoodsInfo();
                if (result == FAIL_TO_OPEN_FILE) {
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

    return EXIT_SUCCESS;
}