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

public:
    OrderedLinkedList();
    ~OrderedLinkedList();

    void addOrdered(const GymVisit& data);

    ListNode* getForwardLinear() const;
    ListNode* getBackwardRecursive() const;
    GymVisit* findAll(const string& targetName, int& outSize) const;
   
    bool tryMarkDeleted(int index);
    bool tryRestore(int index);
    bool remove(int index);
    void removeMarked();

    void clear();

    bool isEmpty() const { return head == nullptr; }
    int getSize() const { return size; }
};