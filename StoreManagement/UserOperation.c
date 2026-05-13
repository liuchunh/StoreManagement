#define _CRT_SECURE_NO_WARNINGS

#include "UserOperation.h"
#include "identity.h" // 借用一下宏定义
#include "AdminOperation.h"
#include "assist.h"
#include "NodeList.h"

int ChangeAccountPassword(Account t_account, const char* NewPassword) {
	if (strcmp(t_account.accountName, ADMIN_ACCOUNT_NAME) == 0) {
		// 不允许修改管理员账号的密码
		return NOT_ALLOWED_OPERATION;
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

	if (strcmp(account[index].password, NewPassword) == 0) {
		// 新密码和旧密码相同 不进行修改
		return SAME_PASSWORD;
	}
	strcpy(account[index].password, NewPassword);
	return SaveAccountInfo();
}

int BuyGoods(int type, const char* goodsName, int amount) {
	GetGoodsInfo(&head);
	char LGoodsName[50] = { 0 };
	strcpy(LGoodsName, goodsName);
	toLower(LGoodsName);

	Node* CurrentNode = head;
	while (head != NULL) {
		char LTempGoodsName[50] = { 0 };
		strcpy(LTempGoodsName, CurrentNode->value.name);
		toLower(LTempGoodsName);
		if (CurrentNode->value.type == type && strcmp(LTempGoodsName, LGoodsName) == 0) {
			if (CurrentNode->value.remaining >= amount) {
				CurrentNode->value.remaining -= amount;
				return SaveGoodsInfo();
			}
			else {
				return NOT_ENOUGH_GOODS;
			}
		}
		CurrentNode = CurrentNode->next;
	}
	return NO_VALID_DATA;
}