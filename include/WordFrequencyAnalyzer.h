#ifndef WORD_FREQUENCY_ANALYZER_H
#define WORD_FREQUENCY_ANALYZER_H

#include "DataStructures/IWordContainer.h"
#include <string>
#include <memory>

class WordFrequencyAnalyzer {
public:
    //构造函数
    WordFrequencyAnalyzer(std::shared_ptr<IWordContainer> container);
    //分析输入文件
    void analyzeFile(const std::string& filePath);
    //输出结果
    void outputResults(const std::string& filePath);

private:
    //工具
    std::shared_ptr<IWordContainer> container;
};

#endif // WORD_FREQUENCY_ANALYZER_H
