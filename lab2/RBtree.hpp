#include <iostream>
#include <string>
#include <cstdint>
#include <fstream>
#pragma once

enum TColour
{
    red,
    black,
};

class TNode
{
public:
    std::string key;
    uint64_t value;
    TColour colour = TColour::black;
    TNode *left = nullptr;
    TNode *right = nullptr;
    TNode *parent = nullptr;
    TNode() = default;
    TNode(std::string key, uint64_t value);
};

class TRBTree
{
public:
    TNode *root = nullptr;
    TNode *nil;
    TRBTree();
    bool Insert(std::string key, uint64_t value);
    bool Erase(std::string key);
    void Save(std::ofstream& file);
    void Load(std::ifstream& file);
    TNode *FindNode(std::string key);
    TNode *GetNil();
    ~TRBTree();
private:
    TNode *FindMinNode(TNode *root);
    TNode *FindMaxNode(TNode *root);
    void Transplant(TNode *a, TNode *b);
    void FixAfterInsert(TNode *node);
    void FixAfterErase(TNode *node);
    void RightRotate(TNode *node);
    void LeftRotate(TNode *node);
    void RecursiveDestroy(TNode *node);
};