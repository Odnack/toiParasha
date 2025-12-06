#include "AVLTreeIndex.h"
#include "GymVisit.h"
#include "GymConsole.h"

AVLNode* AVLTreeIndex::rightRotate(AVLNode* y)
{
	AVLNode* x = y->left;
	AVLNode* temp = x->right;

	x->right = y;
	y->left = temp;

	updateHeight(y);
	updateHeight(x);

	return x;
}

AVLNode* AVLTreeIndex::leftRotate(AVLNode* x)
{
	AVLNode* y = x->right;
	AVLNode* temp = y->left;

	y->left = x;
	x->right = temp;

	updateHeight(x);
	updateHeight(y);

	return y;
}

AVLNode* AVLTreeIndex::balance(AVLNode* node)
{
	if (node == nullptr)
	{
		return nullptr;
	}

	updateHeight(node);
	int balanceFactor = getBalanceFactor(node);

	if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
		return rightRotate(node);
	}

	if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
		return leftRotate(node);
	}

	if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

AVLNode* AVLTreeIndex::insert(AVLNode* node, const string& name, int originalIdx)
{
	if (node == nullptr)
	{
		return new AVLNode(name, originalIdx);
	}

	if (name < node->fullName)
	{
		node->left = insert(node->left, name, originalIdx);
	}
	else
	{
		node->right = insert(node->right, name, originalIdx);
	}

	return balance(node);
}

void AVLTreeIndex::insertRecord(const string& name, int originalIdx)
{
	root = insert(root, name, originalIdx);
}

AVLNode* AVLTreeIndex::findMin(AVLNode* node)
{
	AVLNode* current = node;
	while (current->left != nullptr)
	{
		current = current->left;
	}
	return current;
}

AVLNode* AVLTreeIndex::remove(AVLNode* node, const string& name, int originalIdx)
{
	if (node == nullptr)
	{
		return node;
	}

	if (name < node->fullName)
	{
		node->left = remove(node->left, name, originalIdx);
	}
	else if (name > node->fullName)
	{
		node->right = remove(node->right, name, originalIdx);
	}
	else
	{
		if (node->originalIndex != originalIdx)
		{
			node->right = remove(node->right, name, originalIdx);
			return balance(node);
		}

		if (node->left == nullptr || node->right == nullptr)
		{
			AVLNode* temp = node->left
				? node->left
				: node->right;

			if (temp == nullptr)
			{
				temp = node;
				node = nullptr;
			}
			else
			{
				*node = *temp;
			}
			delete temp;
		}
		else
		{
			AVLNode* temp = findMin(node->right);
			node->fullName = temp->fullName;
			node->originalIndex = temp->originalIndex;
			node->right = remove(node->right, temp->fullName, temp->originalIndex);
		}
	}

	if (node == nullptr)
	{
		return node;
	}

	return balance(node);
}

void AVLTreeIndex::removeRecord(const string& name, int originalIdx)
{
	root = remove(root, name, originalIdx);
}

void AVLTreeIndex::inorderAsc(AVLNode* node, const GymVisit* visits, int& count, void (GymConsole::* displayFunc)(const GymVisit&, int) const, const GymConsole* console) const
{
	if (node == nullptr)
	{
		return;
	}

	inorderAsc(node->left, visits, count, displayFunc, console);
	const GymVisit& visit = visits[node->originalIndex];
	if (!visit.isDeleted) {
		(console->*displayFunc)(visit, count);
		count++;
	}
	inorderAsc(node->right, visits, count, displayFunc, console);
}

void AVLTreeIndex::displayAsc(const GymVisit* visits, const GymConsole* console, void (GymConsole::* displayFunc)(const GymVisit&, int) const) const
{
	int count = 1;
	inorderAsc(root, visits, count, displayFunc, console);
}

void AVLTreeIndex::inorderDesc(AVLNode* node, const GymVisit* visits, int& count, void (GymConsole::* displayFunc)(const GymVisit&, int) const, const GymConsole* console) const
{
	if (node == nullptr)
	{
		return;
	}

	inorderDesc(node->right, visits, count, displayFunc, console);
	const GymVisit& visit = visits[node->originalIndex];
	if (!visit.isDeleted) {
		(console->*displayFunc)(visit, count);
		count++;
	}
	inorderDesc(node->left, visits, count, displayFunc, console);
}

void AVLTreeIndex::displayDesc(const GymVisit* visits, const GymConsole* console, void (GymConsole::* displayFunc)(const GymVisit&, int) const) const
{
	int count = 1;
	inorderDesc(root, visits, count, displayFunc, console);
}

AVLNode* AVLTreeIndex::searchRecursive(AVLNode* node, const string& name) const
{
	if (node == nullptr)
	{
		return nullptr;
	}
	if (name == node->fullName)
	{
		return node;
	}
	if (name < node->fullName)
	{
		return searchRecursive(node->left, name);
	}
	else
	{
		return searchRecursive(node->right, name);
	}
}

AVLNode* AVLTreeIndex::searchIterative(AVLNode* node, const string& name) const
{
	AVLNode* current = node;
	while (current != nullptr)
	{
		if (name == current->fullName)
		{
			return current;
		}
		else if (name < current->fullName)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}
	return nullptr;
}

void AVLTreeIndex::deleteTree(AVLNode* node)
{
	if (node == nullptr) {
		return;
	}

	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}