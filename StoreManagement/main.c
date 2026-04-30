#define _CRT_SECURE_NO_WARNINGS

#include "identity.h"
#include "assist.h"
#include "UserOperation.h"
#include "AdminOperation.h"
#include <stdlib.h>
#include <windows.h>

char* AccountName;
char* Password;

void Init() {
    /* --------- 申请内存 ---------*/
    AccountName = (char*) malloc(100 * sizeof(char));
    Password = (char*) malloc(100 * sizeof(char));

    /* ----------- 变量初始化 -----------*/
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
    // 这里可以添加验证账号和密码的逻辑
    return LOGIN_SUCCESSFULLY; // 假设登录成功
}

int main() {
    Init();

    int LoginRes = Login();
    if (LoginRes == LOGIN_SUCCESSFULLY){ // 身份确定
        char TempAccountName[100] = {0};
        strcpy(TempAccountName, AccountName); // 备份账号名 因为 toLower 会修改它
        toLower(TempAccountName); // 将账号名转换为小写字母 以便进行不区分大小写的比较

        // 如果账号名是管理员账号 则继续验证密码 否则直接确认普通用户身份
        if (strcmp(TempAccountName, ADMIN_ACCOUNT_NAME) == 0){
            if (strcmp(Password, ADMIN_PASSWORD) == 0){
                printf("管理员身份已确认!\n");
                IsAdmin = true;
            }
            else{ // 是管理员账号 但是密码错误
                printf("管理员身份确认失败! 请确定账号密码是否存在!\n");
            }
        }
        else{ // 不是管理员账号
			bool AccountFound = false;
            for (int i = 0; i < AccountCount; i++) {
                if (strcmp(TempAccountName, account[i].accountName) == 0
                    && strcmp(Password, account[i].password) == 0) {
                    IsAdmin = account[i].isAdmin;
                    AccountFound = true;
                    break;
                }
			}
            if (AccountFound) {
                printf("普通用户身份已确认!\n");
            }
            else {
                printf("账号不存在或密码错误!\n");
                return EXIT_FAILURE;
            }
        }
    }
    else {
        printf("登录失败! 请确定账号密码是否存在!\n");
        return EXIT_FAILURE;
    }

    //IsAdmin = true; // 直接设置为管理员身份 以便测试管理员功能
    //memcpy(AccountName, "admin", 6); // 直接设置账号名 以便测试管理员功能

    Sleep(2000);
    system("cls");
    
    printf("当前用户: %s\n", AccountName);
    ShowMenu(IsAdmin);
    int choice = 0;
    printf("请输入您的选择: ");
	int temp = getchar(); // 先读取一个字符 判断是否是数字
    choice = isdigit(temp) ? temp - '0' : 0;
    while (getchar() != '\n'); // 清除输入缓冲区中的换行符 因为 GetAccountName 用的是 fgets 函数 如果不加的话会导致 accountName 为空

    if (IsAdmin) {
        switch (choice) {
            case 1:
            {
                Account NewAccount;
                memset(&NewAccount, 0, sizeof(Account));

                printf("请输入账号：");
                GetAccountName(NewAccount.accountName);
                if (strcmp(ADMIN_ACCOUNT_NAME, NewAccount.accountName) == 0) {
                    printf("不允许修改管理员账号!\n");
                    break;
                }

                bool AccountExist = false;
                for (int i = 0; i < AccountCount; i++) {
                    if (strcmp(account[i].accountName, NewAccount.accountName) == 0) {
                        printf("账号已存在!\n");
                        AccountExist = true;
                        break;
                    }
				}
                if (AccountExist) {
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
				toLower(t);
                if (strcmp(t, ADMIN_ACCOUNT_NAME) == 0) {
                    printf("不允许修改管理员账号!\n");
                    break;
				}

                printf("请输入旧密码：");
				GetAccountPassword(TempAccount.password);
                printf("\n");

				bool VerifyPassed = false;
                for (int i = 0; i < AccountCount; i++) {
                    if (strcmp(account[i].accountName, TempAccount.accountName) == 0
                        && strcmp(account[i].password, TempAccount.password) == 0) {
						// 账号存在且密码正确 允许修改密码
                        memcpy(account[i].password, TempAccount.password, sizeof(TempAccount.password));
                        VerifyPassed = true;
                        break;
                    }
                }

                if (!VerifyPassed) {
                    printf("账号不存在或密码错误!\n");
                    break;
				}

                ResetPassword(TempAccount);
                SaveAccountInfo();
                break;
            }
            case 3:
            {
                SellGoods();
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
                SaveGoodsInfo();
                break;
            }
            case 7:
            {
                LoadGoodsInfo();
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