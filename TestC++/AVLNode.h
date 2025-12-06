#pragma once
#include <string>
using namespace std;

struct AVLNode {
public:
	string fullName;
	int originalIndex;
	int height;
	AVLNode* left;
	AVLNode* right;

	AVLNode(const string& name, int originalIdx);
	~AVLNode();
};