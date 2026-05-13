#include "NodeList.h"

void Insert(Node** head, Good value) {
    Node* NewNode = (Node*) malloc(sizeof(Node));
    if (NewNode == NULL) {
        return;
    }
    GoodsCopy(&NewNode->value, value);
    NewNode->next = NULL;
    if (*head == NULL) {
        *head = NewNode;
        NewNode->value.sign = 1;
        GoodsCount = 1;
    }
    else {
        Node* node = *head;
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = NewNode;
        NewNode->value.sign = node->value.sign + 1;
		GoodsCount++;
    }
}

Node* Delete(Node* head, Good value) {
    Node* node = head;
    Node* prev = NULL;
    if (node != NULL && GoodsEqual(&node->value, &value)) {
        head = node->next;
        UpdateBelowSign(head, -1);
        free(node);
        return head;
    }
    while (node != NULL && !GoodsEqual(&node->value, &value)) {
        prev = node;
        node = node->next;
    }
    if (node == NULL) return head;
    prev->next = node->next;
    UpdateBelowSign(prev->next, -1); // 在删除一个节点后, 需要更新后面的序号
    free(node);
    return head;
}

Node* DeleteByName(Node* head, const char* name) {
    Node* node = head;
    Node* prev = NULL;
    if (node != NULL && strcmp(node->value.name, name) == 0) {
        head = node->next;
        UpdateBelowSign(head, -1);
        free(node);
        return head;
    }
    while (node != NULL && strcmp(node->value.name, name) != 0) {
        prev = node;
        node = node->next;
    }
    if (node == NULL) return head;
    prev->next = node->next;
    UpdateBelowSign(prev->next, -1); // 在删除一个节点后, 需要更新后面的序号
    free(node);
    return head;
}

void Print(Node* head) {
    Node* node = head;
    while (node != NULL) {
        printf("Sign: %d, Name: %s, Price: %.2f, Remaining: %u, Factory: %s, Brand: %s, Type: %s\n",
               node->value.sign,
               node->value.name,
               node->value.price,
               node->value.remaining,
               node->value.factory,
               node->value.brand,
               PrintGoodsType(node->value.type)
        );
        node = node->next;
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
    Node* node = head;
    while (node != NULL) {
        node->value.sign += num;
        node = node->next;
    }
}

void Clear(Node** head) {
    Node* node;
    while (*head != NULL) {
        node = *head;
        *head = (*head)->next;
        free(node);
    }
}