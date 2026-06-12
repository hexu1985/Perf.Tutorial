#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

// 低效的字符串查找（线性搜索）
bool inefficientFind(const std::vector<std::string>& vec, const std::string& target) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return true;
        }
    }
    return false;
}

// 低效的素数判断（重复计算）
bool isPrimeSlow(int n) {
    if (n <= 1) return false;
    for (int i = 2; i < n; ++i) {  // 应该只到 sqrt(n)
        if (n % i == 0) return false;
    }
    return true;
}

// 频繁的内存分配
std::vector<int> createLargeVector(int size) {
    std::vector<int> result;
    for (int i = 0; i < size; ++i) {
        result.push_back(i);  // 多次重新分配
    }
    return result;
}

// 未使用移动语义的字符串拼接
std::string concatenateStrings(const std::vector<std::string>& strs) {
    std::string result;
    for (const auto& s : strs) {
        result = result + s;  // 创建临时对象
    }
    return result;
}

int main() {
    std::cout << "Running unoptimized version...\n";
    
    // 准备测试数据
    std::vector<std::string> data;
    for (int i = 0; i < 10000; ++i) {
        data.push_back("item_" + std::to_string(i));
    }
    
    std::vector<std::string> stringsForConcat;
    for (int i = 0; i < 5000; ++i) {
        stringsForConcat.push_back("str_" + std::to_string(i));
    }
    
    // 执行多次操作以产生足够的采样数据
    for (int iteration = 0; iteration < 100; ++iteration) {
        // 低效查找
        inefficientFind(data, "item_9999");
        
        // 低效素数计算
        for (int i = 0; i < 500; ++i) {
            isPrimeSlow(i);
        }
        
        // 频繁内存分配
        auto vec = createLargeVector(10000);
        
        // 低效字符串拼接
        auto result = concatenateStrings(stringsForConcat);
        
        if (iteration % 20 == 0) {
            std::cout << "Iteration " << iteration << " completed\n";
        }
    }
    
    std::cout << "Unoptimized version finished.\n";
    return 0;
}
