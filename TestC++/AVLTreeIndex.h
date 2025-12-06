#pragma once
#include "AVLNode.h"
#include "GymVisit.h"
#include "GymConsole.h"
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

	void inorderAsc(AVLNode* node, const GymVisit* visits, int& count, void (GymConsole::* displayFunc)(const GymVisit&, int) const, const GymConsole* console) const;
	void inorderDesc(AVLNode* node, const GymVisit* visits, int& count, void (GymConsole::* displayFunc)(const GymVisit&, int) const, const GymConsole* console) const;

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

	void displayAsc(const GymVisit* visits, const GymConsole* console, void (GymConsole::* displayFunc)(const GymVisit&, int) const) const;
	void displayDesc(const GymVisit* visits, const GymConsole* console, void (GymConsole::* displayFunc)(const GymVisit&, int) const) const;
};