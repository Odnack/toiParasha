#pragma once
#include "GymVisit.h"

struct ListNode {
    GymVisit visit;
    ListNode* next;
    ListNode* prev;

    ListNode(const GymVisit& visit) : visit(visit), next(nullptr), prev(nullptr) {}
};