#define _CRT_SECURE_NO_WARNINGS

#include "UserOperation.h"
#include "identity.h" // 借用一下宏定义
#include "AdminOperation.h"
#include "assist.h"

int ChangeAccountPassword(Account t_account, const char* NewPassword) {
	if (strcmp(t_account.accountName, ADMIN_ACCOUNT_NAME) == 0) {
		// 不允许修改管理员账号的密码
		return NOT_ALLOWED_OPERATION;
	}
	if (strcmp(t_account.password, NewPassword) == 0) {
		return SAME_PASSWORD;
	}
	int index = -1;
	Account TempAccount = { 0 };
	strcpy(TempAccount.accountName, t_account.accountName);
	strcpy(TempAccount.password, t_account.password);
	toLower(TempAccount.accountName); // 将账号名转换为小写字母 以便进行不区分大小写的比较

	for (int i = 0; i < AccountCount; i++) {
		Account verify = { 0 };
		strcpy(verify.accountName, account[i].accountName);
		toLower(verify.accountName); 

		if (strcmp(TempAccount.accountName, verify.accountName) == 0) {
			// 名字匹配 继续验证密码
			if (strcmp(TempAccount.password, account[i].password) != 0) {
				return WRONG_PASSWORD;
			}
			else{
				index = i;
				break;
			}
		}
	}

	if (index == -1) {
		return UNEXISTED_ACCOUNT;
	}

	strcpy(account[index].password, NewPassword);
	if (SaveAccountInfo() == OPERATION_SUCCESSFUL) {
		return OPERATION_SUCCESSFUL;
	}
	else {
		return FAIL_TO_WRITE_FILE;
	}
}

int BuyGoods(int type, const char* goodsName, int amount) {
	GetGoodsInfo();
	char LGoodsName[50] = { 0 };
	strcpy(LGoodsName, goodsName);
	toLower(LGoodsName);

	for (int i = 0; i < GoodsCount; i++) {
		char LTempGoodsName[50] = { 0 };
		strcpy(LTempGoodsName, goods[i].name);
		toLower(LTempGoodsName);

		if (goods[i].type == type && strcmp(LTempGoodsName, LGoodsName) == 0) {
			if (goods[i].remaining >= amount) {
				goods[i].remaining -= amount;
				if (SaveGoodsInfo() == OPERATION_SUCCESSFUL) {
					return OPERATION_SUCCESSFUL;
				}
				else {
					return FAIL_TO_WRITE_FILE;
				}
			}
			else {
				return NOT_ENOUGH_GOODS;
			}
		}
	}
	return NO_VALID_DATA;
}