#include "OrderedLinkedList.h"
#include <iostream>

OrderedLinkedList::OrderedLinkedList() : head(nullptr), tail(nullptr), size(0) {}

OrderedLinkedList::~OrderedLinkedList()
{
	clearList();
}

void OrderedLinkedList::clearList()
{
	ListNode* current = head;
	while (current != nullptr) {
		ListNode* next = current->next;
		delete current;
		current = next;
	}
	head = tail = nullptr;
	size = 0;
}

void OrderedLinkedList::addOrdered(const GymVisit& data)
{
	ListNode* newNode = new ListNode(data);

	if (isEmpty())
	{
		head = tail = newNode;
		size++;
		return;
	}

	ListNode* current = head;
	ListNode* prev = nullptr;

	while (current != nullptr)
	{
		if (current->visit.fullName > data.fullName)
		{
			break;
		}

		prev = current;
		current = current->next;
	}

	if (prev == nullptr)
	{
		newNode->next = head;

		if (head != nullptr)
		{
			head->prev = newNode;
		}

		head = newNode;

		if (tail == nullptr)
		{
			tail = newNode;
		}
	}
	else if (current == nullptr)
	{
		prev->next = newNode;
		newNode->prev = prev;
		tail = newNode;
	}
	else
	{
		newNode->next = current;
		newNode->prev = prev;
		prev->next = newNode;
		current->prev = newNode;
	}

	size++;
}

ListNode* OrderedLinkedList::getForwardLinear() const
{
	return head;
}

void OrderedLinkedList::getBackwardRecursiveHelper(ListNode* node, ListNode*& resultHead) const
{
	if (node == nullptr)
	{
		return;
	}

	ListNode* newNode = new ListNode(node->visit);

	if (resultHead != nullptr)
	{
		resultHead->prev = newNode;
	}

	newNode->next = resultHead;
	resultHead = newNode;

	getBackwardRecursiveHelper(node->next, resultHead);
}

ListNode* OrderedLinkedList::getBackwardRecursive() const
{
	ListNode* resultHead = nullptr;

	getBackwardRecursiveHelper(head, resultHead);

	return resultHead;
}

GymVisit* OrderedLinkedList::findAll(const string& targetName, int& outSize) const
{
	GymVisit* resultArray = new GymVisit[size];

	outSize = 0;
	ListNode* current = head;

	while (current != nullptr)
	{
		if (current->visit.fullName == targetName)
		{
			resultArray[outSize] = current->visit;
			outSize++;
		}

		if (current->visit.fullName > targetName)
		{
			break;
		}

		current = current->next;
	}

	return resultArray;
}

void OrderedLinkedList::removeMarked()
{
	ListNode* current = head;

	while (current != nullptr)
	{
		ListNode* nextNode = current->next;

		if (current->visit.isDeleted)
		{
			if (current->prev == nullptr)
			{
				head = current->next;
				if (head != nullptr)
				{
					head->prev = nullptr;
				}
				else
				{
					tail = nullptr;
				}
			}
			else if (current->next == nullptr)
			{
				tail = current->prev;
				tail->next = nullptr;
			}
			else
			{
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}

			delete current;
			size--;
		}

		current = nextNode;
	}
}

void OrderedLinkedList::clear()
{
	clearList();
}

bool OrderedLinkedList::remove(int index)
{
	ListNode* current = head;

	if (index < 0 || index >= size)
	{
		return false;
	}

	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}

	if (current->prev == nullptr) 
	{
		head = current->next;
		if (head != nullptr) 
		{
			head->prev = nullptr;
		}
		else
		{
			tail = nullptr;
		}
	}
	else if (current->next == nullptr) 
	{
		tail = current->prev;
		tail->next = nullptr;
	}
	else 
	{
		current->prev->next = current->next;
		current->next->prev = current->prev;
	}

	delete current;
	size--;
	return true;
}

bool OrderedLinkedList::tryMarkDeleted(int index)
{
	if (index < 0 || index >= size)
	{
		return false;
	}

	ListNode* current = head;
	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}

	if (current->visit.isDeleted)
	{
		return false;
	}

	current->visit.isDeleted = true;
	return true;
}

bool OrderedLinkedList::tryRestore(int index)
{
	if (index < 0 || index >= size)
	{
		return false;
	}

	ListNode* current = head;
	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}

	if (!current->visit.isDeleted)
	{
		return false;
	}

	current->visit.isDeleted = false;
	return true;
}