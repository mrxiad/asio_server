#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include<vector>
#include<string>
namespace StringsUtil {

    // 将字符串分割为多个子字符串
    std::vector<std::string> split(const std::string& str, char delimiter);

    // 去除字符串两端的空格
    std::string trim(const std::string& str);

    // 将字符串转换为小写
    std::string toLower(const std::string& str);

    // 将字符串转换为大写
    std::string toUpper(const std::string& str);

} 
#endif// namespace StringsUtil