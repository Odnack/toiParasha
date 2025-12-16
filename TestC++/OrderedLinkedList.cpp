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

//упорядоченное добавление нового элемента к имеющимся
void OrderedLinkedList::addOrdered(const GymVisit& data)
{
	ListNode* newNode = new ListNode(data); // новый экземпляр структуры (полученный, переданный элемент)

	//bool isEmpty() const { return head == nullptr; } - возвращает флаг наличия списка
	if (isEmpty()) //если список пуст
	{
		head = tail = newNode; //head - первый элемент списка, tail - последний лемент списка
		size++; //size - количество элементов линейного списка
		return;
	}

	ListNode* current = head; //текущий элемент - первый элемент 
	ListNode* prev = nullptr; //предыдущий элемент пуст

	while (current != nullptr) //если текущий элемент существует
	{
		if (current->visit.fullName > data.fullName) //если ФИО текущего больше, чем ФИО полученного
		{
			break; //выход из цикла
		}

		prev = current; //предыдущий элемент = текущему
		current = current->next; // текущий элемент = следующему
	}

	if (prev == nullptr) //если предыдущего элемента нет
	{
		newNode->next = head; // следующий элемент для полученного становится бывшим первым

		if (head != nullptr) //если первый элемент существует (не пустой)
		{
			head->prev = newNode; //полученный занимает место предыдущего элемента
		}

		head = newNode; 

		if (tail == nullptr) //если последний элемент пуст
		{
			tail = newNode; //то полученный занимает место последнего
		}
	}
	else if (current == nullptr) //если текущего элемента нет
	{
		//задаются ссылки для полученного элемента и предыдущего, сам поулченный становится последним
		prev->next = newNode; 
		newNode->prev = prev; 
		tail = newNode; 
	}
	else
	{
		// для текущего элемента задаются следующий и предыдущий элементы, 
		// и для предыдущего и следующего идёт перезапись на текущий
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

//метод получения списка в порядке убывания
ListNode* OrderedLinkedList::getBackwardRecursive() const
{
	ListNode* resultHead = nullptr;

	getBackwardRecursiveHelper(head, resultHead);

	return resultHead;
}

//рекурсивный метод перестройки спсика в порядке убывания
void OrderedLinkedList::getBackwardRecursiveHelper(ListNode* node, ListNode*& resultHead) const
{
	if (node == nullptr) //если посещения нет, то выход из метода
	{
		return;
	}

	ListNode* newNode = new ListNode(node->visit); //запись посещения в новое посещение

	//если переданный элемент resultHead не пуст, тогда задаём предыдущее значение для него - полученное посещение node
	if (resultHead != nullptr) 
	{
		resultHead->prev = newNode;
	}

	//задаём для нового посещения следующий элемент - переданный resultHead	
	newNode->next = resultHead;
	resultHead = newNode; //resultHead становится текущмим зачением

	getBackwardRecursiveHelper(node->next, resultHead); //рекурсивный вызов со следующими элментами списка
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