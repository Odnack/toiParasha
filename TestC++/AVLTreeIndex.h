#pragma once
#include "AVLNode.h"
#include "GymVisit.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class AVLTreeIndex {
private:
	AVLNode* root;

	int getHeight(AVLNode* node) const {
		return node ? node->height : 0;
	}

	int getBalanceFactor(AVLNode* node) const {
		return node ? (getHeight(node->left) - getHeight(node->right)) : 0;
	}

	void updateHeight(AVLNode* node) {
		if (node) {
			node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		}
	}

	AVLNode* rightRotate(AVLNode* y);
	AVLNode* leftRotate(AVLNode* x);

	AVLNode* insert(AVLNode* node, const string& name, int originalIdx);

	AVLNode* findMin(AVLNode* node);
	AVLNode* remove(AVLNode* node, const string& name, int originalIdx);

	AVLNode* balance(AVLNode* node);

	void collectAscOrder(AVLNode* node, GymVisit* visits, GymVisit* result, int& index) const;
	void collectDescOrder(AVLNode* node, GymVisit* visits, GymVisit* result, int& index) const;

	int countNodes(AVLNode* node) const;

	AVLNode* searchRecursive(AVLNode* node, const string& name) const;
	AVLNode* searchIterative(AVLNode* node, const string& name) const;

	void deleteTree(AVLNode* node);

public:
	AVLTreeIndex() : root(nullptr) {}
	~AVLTreeIndex()
	{
		deleteTree(root);
	}

	void insertRecord(const string& name, int originalIdx);
	void removeRecord(const string& name, int originalIdx);
	void clear()
	{
		deleteTree(root);
		root = nullptr;
	}

	AVLNode* findRecursive(const string& name) const { return searchRecursive(root, name); }
	AVLNode* findIterative(const string& name) const { return searchIterative(root, name); }

	GymVisit* getAscOrdered(GymVisit* visits, int& outSize) const;
	GymVisit* getDescOrdered(GymVisit* visits, int& outSize) const;
};