下面给出一个完整的性能优化前后C++示例，以及如何生成差分火焰图来可视化优化效果。

## 1. 性能优化前的代码 (example_before.cpp)

```cpp
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
```

## 2. 性能优化后的代码 (example_after.cpp)

```cpp
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
```

## 3. 编译两个版本

```bash
# 编译优化前的版本
g++ -g -O2 -std=c++11 -o example_before example_before.cpp

# 编译优化后的版本
g++ -g -O2 -std=c++11 -o example_after example_after.cpp
```

## 4. 生成差分火焰图的完整脚本

创建 `generate_diff_flamegraph.sh`：

```bash
#!/bin/bash

# 设置参数
SAMPLE_FREQ=99
DURATION=30  # 采样持续时间（秒）
FLAMEGRAPH_DIR="./FlameGraph"

# 检查FlameGraph工具
if [ ! -d "$FLAMEGRAPH_DIR" ]; then
    echo "Downloading FlameGraph tools..."
    git clone https://github.com/brendangregg/FlameGraph
fi

# 函数：采样并生成折叠文件
sample_and_collapse() {
    local binary=$1
    local output_prefix=$2
    
    echo "Sampling $binary..."
    
    # 运行程序并采样
    sudo perf record -F $SAMPLE_FREQ -g --call-graph dwarf \
        -o "${output_prefix}.data" -- ./$binary
    
    # 生成perf script输出
    sudo perf script -i "${output_prefix}.data" > "${output_prefix}.perf"
    
    # 折叠调用栈
    $FLAMEGRAPH_DIR/stackcollapse-perf.pl "${output_prefix}.perf" > "${output_prefix}.folded"
    
    echo "Generated ${output_prefix}.folded"
}

# 清理旧文件
rm -f before.folded after.folded diff.svg out.perf before.perf after.perf

# 采样优化前版本
sample_and_collapse "example_before" "before"

# 采样优化后版本
sample_and_collapse "example_after" "after"

# 生成差分火焰图
echo "Generating differential flame graph..."
$FLAMEGRAPH_DIR/difffolded.pl before.folded after.folded | \
    $FLAMEGRAPH_DIR/flamegraph.pl --title="Performance Optimization: Before vs After" \
    --colors=diff > diff_flamegraph.svg

echo "Differential flame graph generated: diff_flamegraph.svg"
```

## 5. 简化版差分脚本（手动控制）

```bash
#!/bin/bash

# 步骤1：分别采集优化前后的性能数据
echo "Collecting before optimization data..."
sudo perf record -F 99 -g --call-graph dwarf -o perf_before.data -- ./example_before

echo "Collecting after optimization data..."
sudo perf record -F 99 -g --call-graph dwarf -o perf_after.data -- ./example_after

# 步骤2：生成折叠文件
echo "Generating folded stacks..."
sudo perf script -i perf_before.data | ./FlameGraph/stackcollapse-perf.pl > before.folded
sudo perf script -i perf_after.data | ./FlameGraph/stackcollapse-perf.pl > after.folded

# 步骤3：生成差分火焰图
echo "Generating differential flame graph..."
./FlameGraph/difffolded.pl before.folded after.folded | \
    ./FlameGraph/flamegraph.pl --colors=diff > diff.svg

echo "Open diff.svg in browser to see the optimization impact"
```

## 6. 解读差分火焰图

生成 `diff.svg` 后，颜色含义：

- **红色**：优化后CPU占用**增加**的区域（需要关注）
- **蓝色**：优化后CPU占用**减少**的区域（优化成功）
- **灰色**：没有变化

### 预期看到的结果：

1. **`isPrimeSlow`** 应该显示为**蓝色**（优化后时间减少）
2. **`createLargeVector`** 应该显示为**蓝色**（预分配内存减少分配开销）
3. **`concatenateStrings`** 应该显示为**蓝色**（移动语义和预分配优化）
4. 底部 `main` 函数可能有**少量红色**（因为优化后可能做了更多工作）

## 7. 快速性能验证（不生成火焰图）

```cpp
// 简单的性能测试脚本
#include <chrono>
#include <iostream>

// ... 包含上面的函数定义 ...

template<typename Func>
double measureTime(Func f, const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();
    std::cout << name << ": " << duration << " seconds\n";
    return duration;
}

int main() {
    // 测试优化前后的性能差异
    std::cout << "=== Performance Comparison ===\n";
    
    // 测试数据准备...
    // 运行相同操作，比较时间
    
    return 0;
}
```

## 8. 注意事项

1. **采样时长**：确保程序运行足够长（至少几秒）以获得有效样本
2. **符号表**：使用 `-g` 编译确保函数名可见
3. **优化级别**：保持优化前后的编译选项一致（都使用 `-O2`）
4. **背景噪音**：关闭其他CPU密集型程序
5. **多次采样**：对关键优化建议进行2-3次采样以确保结果稳定

通过差分火焰图，你可以直观地看到：
- `isPrimeSlow` 的蓝色条带说明优化成功
- `createLargeVector` 蓝色表明内存分配减少
- 任何意外的红色区域可能需要进一步分析
