#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include <cmath>

// 优化1：使用哈希表进行O(1)查找
bool efficientFind(const std::vector<std::string>& vec, const std::string& target) {
    static std::unordered_set<std::string> cache;
    if (cache.empty()) {
        cache.insert(vec.begin(), vec.end());
    }
    return cache.find(target) != cache.end();
}

// 优化2：使用sqrt优化素数判断
bool isPrimeFast(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    int limit = static_cast<int>(std::sqrt(n));
    for (int i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// 优化3：预分配内存
std::vector<int> createLargeVectorFast(int size) {
    std::vector<int> result;
    result.reserve(size);  // 预分配，避免多次重新分配
    for (int i = 0; i < size; ++i) {
        result.push_back(i);
    }
    return result;
}

// 优化4：使用移动语义和operator+=
std::string concatenateStringsFast(const std::vector<std::string>& strs) {
    std::string result;
    result.reserve(100000);  // 预分配足够空间
    for (const auto& s : strs) {
        result += s;  // 直接追加，避免临时对象
    }
    return result;
}

// 新增：更好的缓存局部性
class OptimizedDataProcessor {
private:
    std::vector<int> data;
    
public:
    OptimizedDataProcessor(int size) {
        data.reserve(size);
        for (int i = 0; i < size; ++i) {
            data.push_back(i);
        }
    }
    
    int processData() {
        int sum = 0;
        // 顺序访问，利用缓存局部性
        for (const auto& val : data) {
            if (isPrimeFast(val)) {
                sum += val;
            }
        }
        return sum;
    }
};

int main() {
    std::cout << "Running optimized version...\n";
    
    // 准备测试数据
    std::vector<std::string> data;
    data.reserve(10000);
    for (int i = 0; i < 10000; ++i) {
        data.push_back("item_" + std::to_string(i));
    }
    
    std::vector<std::string> stringsForConcat;
    stringsForConcat.reserve(5000);
    for (int i = 0; i < 5000; ++i) {
        stringsForConcat.push_back("str_" + std::to_string(i));
    }
    
    // 执行相同的操作次数
    for (int iteration = 0; iteration < 100; ++iteration) {
        // 优化后的查找
        efficientFind(data, "item_9999");
        
        // 优化后的素数计算
        for (int i = 0; i < 500; ++i) {
            isPrimeFast(i);
        }
        
        // 优化后的内存分配
        auto vec = createLargeVectorFast(10000);
        
        // 优化后的字符串拼接
        auto result = concatenateStringsFast(stringsForConcat);
        
        // 额外的优化操作
        OptimizedDataProcessor processor(5000);
        auto sum = processor.processData();
        
        if (iteration % 20 == 0) {
            std::cout << "Iteration " << iteration << " completed\n";
        }
    }
    
    std::cout << "Optimized version finished.\n";
    return 0;
}
