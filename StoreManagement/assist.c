#define _CRT_SECURE_NO_WARNINGS

#include "assist.h"
#include "AdminOperation.h"

/**
 * @brief 将 str 中 所有的大写字母转换为小写字母
 *
 * @param str
 * @note 函数会直接修改 str 中的内容, 调用前请先备份 str
 */
void toLower(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (isupper(str[i])) {
            str[i] = tolower(str[i]);
        }
    }
}

/**
 * @brief 将 str 转换为数字
 *
 * @param str
 * @return int 返回值
 *
 * @note 如果 str 中包含非数字字符, 则返回 NOT_DIGIT
 */
int str2int(const char* str) {
    int len = strlen(str);
    int num = 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return NOT_DIGIT;
        }
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

/**
 * @brief 将 str 转换为数字
 *
 * @return int 返回值
 *
 * @note 如果 str 中包含非数字字符, 则返回 NOT_DIGIT
 */
int GetIntegerInput() {
    char str[20] = { 0 };
    scanf("%s", str);
    int num = str2int(str);
    return num;
}

/**
 * @brief 将 str 转换为数字
 * 
 * @return double 
 * 
 * @note 如果 str 中包含非数字字符, 则返回 NOT_DIGIT
 */
double GetDoubleInput() {
    char str[20] = { 0 };
	scanf("%s", str);
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return NOT_DIGIT;
        }
    }
	return atof(str);
}

/**
 * @brief 指针函数, 用于 qsort 函数按照价格升序排序
 * 
 * @param a 
 * @param b 
 * @return int 
 * 
 * @note 函数返回值和参数类型不得修改!!!
 * 
 * @example qsort(goods, numGoods, sizeof(Good), PriceAscend); // #include <stdlib.h>
 */
int PriceAscend(const void* a, const void* b){
    Good* goodA = (Good*) a;
    Good* goodB = (Good*) b;
    if (goodA->price < goodB->price) {
        return -1;
    }
    else if (goodA->price > goodB->price) {
        return 1;
    }
    else {
        return 0;
	}
}

/**
 * @brief 指针函数, 用于 qsort 函数按照价格降序排序
 * 
 * @param a 
 * @param b 
 * @return int 
 * 
 * @example qsort(goods, numGoods, sizeof(Good), PriceDescend); // #include <stdlib.h>
 */
int PriceDescend(const void* a, const void* b){
    Good* goodA = (Good*) a;
    Good* goodB = (Good*) b;
    if (goodA->price > goodB->price) {
        return -1;
    }
    else if (goodA->price < goodB->price) {
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * @brief 指针函数, 用于 qsort 函数按照剩余数量升序排序
 * 
 * @param a 
 * @param b 
 * @return int 
 * 
 * @example qsort(goods, numGoods, sizeof(Good), RemainingAscend);
 */
int RemainingAscend(const void* a, const void* b){
    Good* goodA = (Good*) a;
    Good* goodB = (Good*) b;
    if (goodA->remaining < goodB->remaining) {
        return -1;
    }
    else if (goodA->remaining > goodB->remaining) {
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * @brief 指针函数, 用于 qsort 函数按照剩余数量降序排序
 * 
 * @param a 
 * @param b 
 * @return int 
 * 
 * @example qsort(goods, numGoods, sizeof(Good), RemainingDescend);
 */
int RemainingDescend(const void* a, const void* b){
    Good* goodA = (Good*) a;
    Good* goodB = (Good*) b;
    if (goodA->remaining > goodB->remaining) {
        return -1;
    }
    else if (goodA->remaining < goodB->remaining) {
        return 1;
    }
    else {
        return 0;
    }
}