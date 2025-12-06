#pragma once
#include <string>
using namespace std;

struct AVLNode {
public:
	string fullName; 
	int originalIndex;
	int height; // высота поддерева для балансировки
	AVLNode* left; //ссылка на левый элемент
	AVLNode* right; // ссылка на правый элемент

	AVLNode(const string& name, int originalIdx); 
	~AVLNode();
};