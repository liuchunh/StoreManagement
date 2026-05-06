#define _CRT_SECURE_NO_WARNINGS

#include "identity.h"

const int BACKSPACE = 8; // 退格键的ASCII码
const int ENTER = 13; // 回车键的ASCII码
const char ADMIN_ACCOUNT_NAME[] = "admin";
const char ADMIN_PASSWORD[] = "123456";

Account account[100] = { 0 };
int AccountCount = 0;

bool IsAdmin = false;

/**
 * @brief 从 account.csv 文件中读取账号信息并存储到内存中
 * 
 */
void GetAccountInfo() {
    AccountCount = 0;
    FILE* fp = fopen("account.csv", "r");
    if (fp == NULL) {
        printf("无法打开账户文件!\n");
        return;
    }

	bool ModifyAdminPassword = false; // 是否需要修改管理员密码的标志
    while (AccountCount < 100 &&
           fscanf(fp, " %99[^,],%99s",
                  account[AccountCount].accountName, account[AccountCount].password) == 2) {
        account[AccountCount].accountName[strcspn(account[AccountCount].accountName, "\n")] = '\0'; // 去除输入末尾的换行符
        if (strcmp(account[AccountCount].accountName, ADMIN_ACCOUNT_NAME) == 0) {
            if (strcmp(account[AccountCount].password, ADMIN_PASSWORD) != 0) {
				// 管理员账号的密码不正确，强制设置为管理员密码
				ModifyAdminPassword = true;
				printf("Warning: Wrong password for admin account found in file, resetting to default password.\n");
				strcpy(account[AccountCount].password, ADMIN_PASSWORD);
            }
            account[AccountCount].isAdmin = true;
        }
        AccountCount++;
    }
    fclose(fp);

    if (ModifyAdminPassword) {
        // 如果需要修改管理员密码，保存修改后的账号信息到文件
        FILE* fp_write = fopen("account.csv", "w");
        if (fp_write == NULL) {
            printf("无法打开账户文件进行写入!\n");
            fclose(fp);
            return;
        }
        for (int i = 0; i < AccountCount; i++) {
            fprintf(fp_write, "%s,%s\n", account[i].accountName, account[i].password);
        }
        fclose(fp_write);
	}
}

/**
 * @brief 获取用户输入的账号
 *
 * @param accountName 账号名, 结果会直接储存在这里
 *
 * @note 需要先初始化 accountName, 至少 100 * sizeof(char)
 */
void GetAccountName(char* accountName) {
    fgets(accountName, 100, stdin);
    accountName[strcspn(accountName, "\n")] = '\0'; // 去除输入末尾的换行符
}

/**
 * @brief 获取用户输入的密码
 *
 * @param password 密码, 结果会直接储存在这里
 *
 * @note 需要先初始化 password, 至少 100 * sizeof(char)
 */
void GetAccountPassword(char* password) {
    char c = '\0';
    while (c = _getch()) {
        if (c == BACKSPACE) {
            if (password[0] != '\0') {
                // 非空，删除最后一个字符
                password[strlen(password) - 1] = '\0';
                printf("\b \b"); // 删除控制台上的星号
            }
        }
        else if (c == ENTER) {
            return;
        }
        else {
            // 添加字符到密码
            int len = strlen(password);
            password[len] = c;
            password[len + 1] = '\0';
            printf("*"); // 显示星号
        }
    }
}

void ShowMenu(bool IsAdmin) {
    if (IsAdmin) {
        printf("*********************************************\n");
        printf("*              管理员菜单                   *\n");
        printf("*             1. 添加账户                   *\n");
        printf("*             2. 重置账户密码               *\n");
        printf("*             3. 出售商品                   *\n");
        printf("*             4. 商品管理                   *\n");
        printf("*             5. 库存统计                   *\n");
        printf("*             6. 商品信息存盘               *\n");
        printf("*             7. 读入货物信息               *\n");
        printf("*             8. 退出                       *\n");
        printf("*********************************************\n");
    }
    else {
        printf("*********************************************\n");
        printf("*              普通用户菜单                 *\n");
        printf("*             1. 修改账号密码               *\n");
        printf("*             2. 信息查询                   *\n");
        printf("*             3. 购买商品                   *\n");
        printf("*             4. 退出                       *\n");
        printf("**********************************************\n");
    }
}