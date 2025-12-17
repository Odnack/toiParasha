#pragma once
#include "ListNode.h"
#include "GymVisit.h"

class OrderedLinkedList {
private:
    ListNode* head;
    ListNode* tail;
    int size;

    void clearList();
    void getBackwardRecursiveHelper(ListNode* node, ListNode*& resultHead) const;
    void findAllRecursiveHelper(ListNode* current, const string& targetName, GymVisit** resultArray, int& outSize) const;

public:
    OrderedLinkedList();
    ~OrderedLinkedList();

    void addOrdered(const GymVisit& data);

    ListNode* getForwardLinear() const;
    ListNode* getBackwardRecursive() const;
    GymVisit** findAll(const string& targetName, int& outSize) const;
    GymVisit** findAllRecursive(const string& targetName, int& outSize) const;

    bool tryRestore(int index);
    void removeMarked();

    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return size; }
};