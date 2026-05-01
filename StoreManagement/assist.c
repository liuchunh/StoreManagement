#include "assist.h"

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

int GetIntegerInput() {
    char str[100] = { 0 };
    scanf("%s", str);
    int num = str2int(str);
    return num;
}