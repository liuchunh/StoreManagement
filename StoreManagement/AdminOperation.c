#define _CRT_SECURE_NO_WARNINGS

#include "AdminOperation.h"

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

void ResetPassword(Account t_account) {
    for (int i = 0; i < AccountCount; i++) {
        if (strcmp(account[i].accountName, t_account.accountName) == 0) {
			memset(account[i].password, 0, sizeof(account[i].password));
			printf("请输入新密码：");
            GetAccountPassword(account[i].password);
            break;
        }
	}
}

void SellGoods() {

}

void ManageGoods() {

}

void InventoryStatistics() {

}

void SaveGoodsInfo() {

}

void LoadGoodsInfo() {

}