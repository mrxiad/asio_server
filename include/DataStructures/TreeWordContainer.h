#ifndef TREE_WORD_CONTAINER_H
#define TREE_WORD_CONTAINER_H

#include <string>
#include "IWordContainer.h"

typedef struct TreeNode{
    std::string data;
    unsigned int frequency;
    TreeNode* left;
    TreeNode* right;
}TreeNode,*PTreeNode;

//抽象类
class TREEWordContainer :public IWordContainer{
public:
    TREEWordContainer();
    virtual void addWord(const std::string& word);
    virtual unsigned int getWordFrequency(const std::string& word) const ;
    virtual ~TREEWordContainer() {}
private:
    PTreeNode root;
};


#endif // TREE_WORD_CONTAINER_H
