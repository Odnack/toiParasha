#include "AVLTreeIndex.h"
#include "GymVisit.h"

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
	//getBalanceFactor(node) возвращает разницу левой и правой ноды
	int balanceFactor = getBalanceFactor(node);

	if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
		return rightRotate(node); //поворачивает дерево направо
	}

	if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
		node->left = leftRotate(node->left);
		return rightRotate(node); //поворачивает дерево направо
	}

	if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
		return leftRotate(node); //поворачивает дерево налево
	}

	if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
		node->right = rightRotate(node->right);
		return leftRotate(node); //поворачивает дерево налево
	}

	return node;
}

//метод, вызывающий добавление элемента в индекс
void AVLTreeIndex::insertRecord(const string& name, int originalIdx)
{
	root = insert(root, name, originalIdx);
}

//рекурсивное добавление элемента в индекс
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

	return balance(node); //вызов балансировки
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

void AVLTreeIndex::collectAscOrder(AVLNode* node, GymVisit* visits, GymVisit* result, int& index) const
{
	if (node == nullptr)
	{
		return;
	}

	collectAscOrder(node->left, visits, result, index);
	GymVisit& visit = visits[node->originalIndex];
	if (!visit.isDeleted)
	{
		result[index++] = visit;
	}
	collectAscOrder(node->right, visits, result, index);
}

void AVLTreeIndex::collectDescOrder(AVLNode* node, GymVisit* visits, GymVisit* result, int& index) const
{
	if (node == nullptr)
	{
		return;
	}

	collectDescOrder(node->right, visits, result, index);
	GymVisit& visit = visits[node->originalIndex];
	if (!visit.isDeleted)
	{
		result[index++] = visit;
	}
	collectDescOrder(node->left, visits, result, index);
}

GymVisit* AVLTreeIndex::getAscOrdered(GymVisit* visits, int& outSize) const
{
	int totalCount = countNodes(root);

	GymVisit* result = new GymVisit[totalCount];

	int index = 0;
	collectAscOrder(root, visits, result, index);

	outSize = index;

	return result;
}

GymVisit* AVLTreeIndex::getDescOrdered(GymVisit* visits, int& outSize) const
{
	int totalCount = countNodes(root);

	GymVisit* result = new GymVisit[totalCount];

	int index = 0;
	collectDescOrder(root, visits, result, index);

	outSize = index;

	return result;
}

int AVLTreeIndex::countNodes(AVLNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}
	return 1 + countNodes(node->left) + countNodes(node->right);
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