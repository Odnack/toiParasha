#pragma once
#include <string>
using namespace std;

struct AVLNode {
    string fullName;
    int originalIndex;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const string& name, int originalIdx);
};


AVLNode::AVLNode(const string& name, int originalIdx)
    : fullName(name), originalIndex(originalIdx), height(1), left(nullptr), right(nullptr) {
}