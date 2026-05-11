#include "NodeList.h"

void Insert(Node** head, Good value) {
    Node* NewNode = (Node*) malloc(sizeof(Node));
    if (NewNode == NULL) {
        // 分配内存失败，直接返回，不做插入
        return;
    }
    GoodsCopy(&NewNode->value, value);
    NewNode->next = NULL;
    if (*head == NULL) {
        *head = NewNode;
        NewNode->value.sign = 1;
    }
    else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = NewNode;
        NewNode->value.sign = temp->value.sign + 1;
    }
}

Node* Delete(Node* head, Good value) {
    Node* temp = head;
    Node* prev = NULL;
    if (temp != NULL && GoodsEqual(&temp->value, &value)) {
        head = temp->next;
        UpdateBelowSign(head, -1);
        free(temp);
        return head;
    }
    while (temp != NULL && !GoodsEqual(&temp->value, &value)) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return head;
    prev->next = temp->next;
    UpdateBelowSign(prev->next, -1); // 在删除一个节点后, 需要更新后面的序号
    free(temp);
    return head;
}

Node* DeleteByName(Node* head, const char* name) {
    Node* temp = head;
    Node* prev = NULL;
    if (temp != NULL && strcmp(temp->value.name, name) == 0) {
        head = temp->next;
        UpdateBelowSign(head, -1);
        free(temp);
        return head;
    }
    while (temp != NULL && strcmp(temp->value.name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return head;
    prev->next = temp->next;
    UpdateBelowSign(prev->next, -1); // 在删除一个节点后, 需要更新后面的序号
    free(temp);
    return head;
}

void Print(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("Sign: %d, Name: %s, Price: %.2f, Remaining: %u, Factory: %s, Brand: %s, Type: %s\n",
               temp->value.sign,
               temp->value.name,
               temp->value.price,
               temp->value.remaining,
               temp->value.factory,
               temp->value.brand,
               PrintGoodsType(temp->value.type)
        );
        temp = temp->next;
    }
    printf("\n");
}

/**
 * @brief 在添加或者删除链表数据后 更新后面商品数据的序号
 *
 * @param head
 * @param num
 *
 * @example insert -> UpdateBelowSign(head, 1) \
 * 删除 -> UpdateBelowSign(head, -1)
 */
void UpdateBelowSign(Node* head, int num) {
    Node* temp = head;
    while (temp != NULL) {
        temp->value.sign += num;
        temp = temp->next;
    }
}

void Free(Node* head) {
    Node* temp = NULL;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}