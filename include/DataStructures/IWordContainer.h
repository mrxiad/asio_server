#ifndef I_WORD_CONTAINER_H
#define I_WORD_CONTAINER_H

#include <string>

//抽象类
class IWordContainer {
public:
    //添加一个单词
    virtual void addWord(const std::string& word) = 0;
    //获取word的词频
    virtual unsigned int getWordFrequency(const std::string& word) const = 0;
    virtual ~IWordContainer() {}
};

#endif // I_WORD_CONTAINER_H
