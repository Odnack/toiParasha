#pragma once
#include "GymVisit.h"

struct ListNode {
    GymVisit visit; //посещение
    ListNode* next; //ссылка на следующее посещение
    ListNode* prev; //ссылка на предыдущее посещение

    ListNode(const GymVisit& visit) : visit(visit), next(nullptr), prev(nullptr) {} //конструктор
};