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