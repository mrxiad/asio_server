#ifndef LIST_WORD_CONTAINER_H
#define LIST_WORD_CONTAINER_H

#include <string>
#include "IWordContainer.h"

typedef struct ListNode {
    std::string data;
    unsigned int frequency;
    ListNode* next;
}ListNode,*PListNode;
//抽象类
class LISTWordContainer :public IWordContainer{
public:
    //初始化p
    LISTWordContainer();
    virtual void addWord(const std::string& word);
    virtual unsigned int getWordFrequency(const std::string& word) const ;
    virtual ~LISTWordContainer() {}
private:
    PListNode p;
};

#endif // LIST_WORD_CONTAINER_H
