#include "AVLNode.h"

AVLNode::AVLNode(const string& name, int originalIdx)
    : fullName(name), originalIndex(originalIdx), height(1), left(nullptr), right(nullptr) {
}