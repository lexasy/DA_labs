#include "RBtree.hpp"
#include "Queue.hpp"

TNode::TNode(std::string key, uint64_t value)
{
    this->key = key;
    this->value = value;
    this->colour = TColour::red;
}

TRBTree::TRBTree()
{
    nil = new TNode();
    root = nil;
}

TRBTree::~TRBTree()
{
    RecursiveDestroy(root);
    delete nil;
}

void TRBTree::RecursiveDestroy(TNode *node)
{
    if (node != nil)
    {
        RecursiveDestroy(node->left);
        RecursiveDestroy(node->right);
        delete node;
    }
}

TNode *TRBTree::GetNil()
{
    return nil;
}

TNode *TRBTree::FindMinNode(TNode *root)
{
    TNode *currentNode = root;
    while (currentNode->left != nil)
    {
        currentNode = currentNode->left;
    }
    return currentNode;
}

TNode *TRBTree::FindMaxNode(TNode *root)
{
    TNode *currentNode = root;
    while (currentNode->right != nil)
    {
        currentNode = currentNode->right;
    }
    return currentNode;
}

TNode *TRBTree::FindNode(std::string key)
{
    TNode *currentNode = this->root;
    while (currentNode->key != key)
    {
        if (currentNode == nil)
        {
            break;
        }
        currentNode = (key < currentNode->key) ? currentNode->left : currentNode->right;
    }
    return currentNode;
}

void TRBTree::LeftRotate(TNode *node)
{
    TNode *tmp = node->right; // set tmp
    node->right = tmp->left; // turn tmp's left subtree into node's right subtree
    if (tmp->left != nil)
    {
        tmp->left->parent = node;
    }
    tmp->parent = node->parent; // link node's parent to tmp
    if (node->parent == nil)
    {
        this->root = tmp;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = tmp;
    }
    else
    {
        node->parent->right = tmp;
    }
    tmp->left = node; // put node on tmp's left
    node->parent = tmp;
}

void TRBTree::RightRotate(TNode *node)
{
    TNode *tmp = node->left; // set tmp
    node->left = tmp->right; // turn tmp's right subtree into node's left subtree
    if (tmp->right != nil)
    {
        tmp->right->parent = node;
    }
    tmp->parent = node->parent; // link node's parent to tmp
    if (node->parent == nil)
    {
        this->root = tmp;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = tmp;
    }
    else
    {
        node->parent->right = tmp;
    }
    tmp->right = node; // put node on tmp's right
    node->parent = tmp;
}

// Case 0: new node is root -> Solution: set black colour in new node
// Case 1: uncle of new node has red colour -> Solution: recolour parent, grandparent and uncle
// Case 2: uncle of new node has black colour (triangle) -> Solution: rotate parent of new node in side which opposite for new node side
// Case 3: uncle of new node has black colour (line) -> Solution: rotate parent of new node in side which opposite for new node side and recolour parent and grandparent

void TRBTree::FixAfterInsert(TNode *node)
{
    TNode *uncle;
    while (node->parent->colour == TColour::red)
    {
        if (node->parent == node->parent->parent->left)
        {
            uncle = node->parent->parent->right;
            if (uncle->colour == TColour::red)
            {
                node->parent->colour = TColour::black; // Case 1
                uncle->colour = TColour::black; // Case 1
                node->parent->parent->colour = TColour::red; // Case 1
                node = node->parent->parent; // Case 1
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent; // Case 2
                    this->LeftRotate(node); // Case 2
                }
                node->parent->colour = TColour::black; // Case 3
                node->parent->parent->colour = TColour::red; // Case 3
                this->RightRotate(node->parent->parent); // Case 3
            }
        }
        else
        {
            uncle = node->parent->parent->left;
            if (uncle->colour == TColour::red)
            {
                node->parent->colour = TColour::black; // Case 1
                uncle->colour = TColour::black; // Case 1
                node->parent->parent->colour = TColour::red; // Case 1
                node = node->parent->parent; // Case 1
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent; // Case 2
                    this->RightRotate(node); // Case 2
                }
                node->parent->colour = TColour::black; // Case 3
                node->parent->parent->colour = TColour::red; // Case 3
                this->LeftRotate(node->parent->parent); // Case 3
            }
        }
    }
    this->root->colour = TColour::black; // Case 0
}

bool TRBTree::Insert(std::string key, uint64_t value)
{
    TNode *currentNode = this->root;
    TNode *parentNode = nil;
    while (currentNode != nil)
    {
        if (currentNode->key == key)
        {
            return false;
        }
        parentNode = currentNode;
        currentNode = (key < currentNode->key) ? currentNode->left : currentNode->right;
    }
    TNode *newNode = new TNode(key, value);
    newNode->parent = parentNode;
    newNode->left = nil;
    newNode->right = nil;
    if (parentNode == nil)
    {
        this->root = newNode;
    }
    else if (key < parentNode->key)
    {
        parentNode->left = newNode;
    }
    else
    {
        parentNode->right = newNode;
    }
    this->FixAfterInsert(newNode);
    return true;
}

void TRBTree::Transplant(TNode *a, TNode *b)
{
    if (a->parent == nil)
    {
        this->root = b;
    }
    else if (a == a->parent->left)
    {
        a->parent->left = b;
    }
    else
    {
        a->parent->right = b;
    }
    b->parent = a->parent;
}

// Case 1: sibling is red -> Solution: 
// Case 2: sibling is black and his children are black too -> Solution: 
// Case 3: sibling is black and his left child is red, right child is black -> Solution: 
// Case 4: sibling is black and his right child is red -> Solution: 

void TRBTree::FixAfterErase(TNode *node)
{
    while ((node != this->root) && (node->colour == TColour::black))
    {
        if (node == node->parent->left)
        {
            TNode *sibling = node->parent->right;
            if (sibling->colour == TColour::red) // Case 1
            {
                sibling->colour = TColour::black; // Case 1
                node->parent->colour = TColour::red; // Case 1
                this->LeftRotate(node->parent); // Case 1
                sibling = node->parent->right; // Case 1
            }
            if ((sibling->left->colour == TColour::black) && (sibling->right->colour == TColour::black)) // Case 2
            {
                sibling->colour = TColour::red; // Case 2
                node = node->parent; // Case 2
            }
            else
            {
                if (sibling->right->colour == TColour::black) // Case 3
                {
                    sibling->left->colour = TColour::black; // Case 3
                    sibling->colour = TColour::red; // Case 3
                    this->RightRotate(sibling); // Case 3
                    sibling = node->parent->right; // Case 3
                }
                sibling->colour = node->parent->colour; // Case 4
                node->parent->colour = TColour::black; // Case 4
                sibling->right->colour = TColour::black; // Case 4
                this->LeftRotate(node->parent); // Case 4
                node = this->root; // Case 4
            }
        }
        else
        {
            TNode *sibling = node->parent->left;
            if (sibling->colour == TColour::red) // Case 1
            {
                sibling->colour = TColour::black; // Case 1
                node->parent->colour = TColour::red; // Case 1
                this->RightRotate(node->parent); // Case 1
                sibling = node->parent->left; // Case 1
            }
            if ((sibling->left->colour == TColour::black) && (sibling->right->colour == TColour::black)) // Case 2
            {
                sibling->colour = TColour::red; // Case 2
                node = node->parent; // Case 2
            }
            else
            {
                if (sibling->left->colour == TColour::black) // Case 3
                {
                    sibling->right->colour = TColour::black; // Case 3
                    sibling->colour = TColour::red; // Case 3
                    this->LeftRotate(sibling); // Case 3
                    sibling = node->parent->left; // Case 3
                }
                sibling->colour = node->parent->colour; // Case 4
                node->parent->colour = TColour::black; // Case 4
                sibling->left->colour = TColour::black; // Case 4
                this->RightRotate(node->parent); // Case 4
                node = this->root; // Case 4
            }
        }
    }
    node->colour = TColour::black;
}

// Case 1: left child is nil -> Solution: transplantation of delete node and her right child
// Case 2: right child is nil -> Solution: transplantation of delete node and her left child
// Case 3: neither child is nil -> Solution: find minimum in right subtree, transplant minimum and his right child, transplant delete node and minimum

bool TRBTree::Erase(std::string key)
{
    TNode *deleteNode = this->FindNode(key);
    TNode *tmp;
    TColour originalColour = deleteNode->colour;
    if (deleteNode == nil)
    {
        return false;
    }
    if (deleteNode->left == nil)
    {
        tmp = deleteNode->right; // Case 1
        this->Transplant(deleteNode, deleteNode->right); // Case 1
        delete deleteNode; // Case 1
    }
    else if (deleteNode->right == nil)
    {
        tmp = deleteNode->left; // Case 2
        this->Transplant(deleteNode, deleteNode->left); // Case 2
        delete deleteNode; // Case 2
    }
    else
    {
        TNode *rightMinimum = this->FindMinNode(deleteNode->right); // Case 3
        originalColour = rightMinimum->colour; // Case 3
        tmp = rightMinimum->right; // Case 3
        if (rightMinimum->parent == deleteNode) // Case 3
        {
            tmp->parent = rightMinimum; // Case 3
        }
        else
        {
            this->Transplant(rightMinimum, rightMinimum->right); // Case 3
            rightMinimum->right = deleteNode->right; // Case 3
            rightMinimum->right->parent = rightMinimum; // Case 3
        }
        this->Transplant(deleteNode, rightMinimum); // Case 3
        rightMinimum->left = deleteNode->left; // Case 3
        rightMinimum->left->parent = rightMinimum; // Case 3
        rightMinimum->colour = deleteNode->colour; // Case 3
        delete deleteNode; // Case 3
    }
    if (originalColour == TColour::black)
    {
        this->FixAfterErase(tmp);
    }
    return true;
}

void TRBTree::Save(std::ofstream& file)
{
    if (root != nil)
    {
        TQueue<TNode *> level;
        level.PushBack(root);
        TQueue<TNode *> next_level;
        while (level.size != 0)
        {
            next_level.Clear();
            for (size_t i = 0; i < level.size; ++i)
            {
                file << level[i]->key << " " << level[i]->value << "\n";
                if (level[i]->left != nil)
                {
                    next_level.PushBack(level[i]->left);
                }
                if (level[i]->right != nil)
                {
                    next_level.PushBack(level[i]->right);
                }
            }
            level.Clear();
            for (size_t i = 0; i < next_level.size; ++i)
            {
                level.PushBack(next_level[i]);
            }
        }
    }
}

void TRBTree::Load(std::ifstream& file)
{
    this->RecursiveDestroy(this->root);
    this->root = nil;
    std::string fkey;
    uint64_t fvalue;
    while (file >> fkey >> fvalue)
    {
        this->Insert(fkey, fvalue);
    }
}