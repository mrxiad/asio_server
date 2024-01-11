#ifndef HASH_WORD_CONTAINER_H
#define HASH_WORD_CONTAINER_H

#include <string>
#include "IWordContainer.h"
typedef struct HashNode {
    std::string data;
    unsigned int frequency;
    HashNode* next;
}HashNode,*PHashNode;
//抽象类
class HashWordContainer :public IWordContainer{
public:
    //构造函数
    HashWordContainer();
    virtual void addWord(const std::string& word);
    virtual unsigned int getWordFrequency(const std::string& word) const ;
    virtual ~HashWordContainer() {}
private:
    PHashNode p;
};

#endif // HASH_WORD_CONTAINER_H
