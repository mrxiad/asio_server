#ifndef Array_WORD_CONTAINER_H
#define Array_WORD_CONTAINER_H

#include <string>
#include "IWordContainer.h"
typedef struct ArrayNode {
    std::string data;
    unsigned int frequency;
    ArrayNode* next;
}ArrayNode,*PArrayNode;
//抽象类
class ArrayWordContainer :public IWordContainer{
public:
    //构造函数
    ArrayWordContainer();
    virtual void addWord(const std::string& word);
    virtual unsigned int getWordFrequency(const std::string& word) const ;
    virtual ~ArrayWordContainer() {}
private:
    PArrayNode p;
};

#endif // Array_WORD_CONTAINER_H
