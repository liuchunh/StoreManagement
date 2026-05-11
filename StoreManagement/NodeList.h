#pragma once

/**
 * @file NodeList.h
 * @author Ma Xuanxuan
 * @brief Node List Operations for Store Management System (Information of goods)
 * @version 0.1
 * @date 2026-05-11
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "AdminOperation.h"

typedef struct _goods_node {
    Good value;
    struct _goods_node* next;
} Node;

void UpdateBelowSign(Node* head, int num);

void Insert(Node** head, Good value);

Node* Delete(Node* head, Good value);

Node* DeleteByName(Node* head, const char* name);

void Print(Node* head);

void Free(Node* head);