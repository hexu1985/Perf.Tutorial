下面给出一个完整的C++示例，演示如何用 `perf` 采集性能数据并生成CPU火焰图。

## 1. C++ 示例代码

创建 `example.cpp`：

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

// 消耗CPU的函数1：计算sin/cos
void computeTrig(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        double x = std::sin(i) * std::cos(i);
        volatile double y = x; // 防止被优化
    }
}

// 消耗CPU的函数2：进行大量整数运算
void computeIntegers(int iterations) {
    volatile long result = 0;
    for (int i = 0; i < iterations; ++i) {
        result += i * i;
    }
}

// 消耗CPU的函数3：递归函数
int recursiveSum(int n) {
    if (n <= 0) return 0;
    volatile int temp = n;
    return recursiveSum(n - 1) + temp;
}

int main() {
    std::cout << "Starting CPU-intensive workload...\n";
    
    for (int round = 0; round < 10; ++round) {
        // 调用不同函数产生多样化的调用栈
        computeTrig(10000000);
        computeIntegers(100000000);
        recursiveSum(5000);
        
        if (round % 2 == 0) {
            std::cout << "Round " << round << " completed\n";
        }
    }
    
    std::cout << "Workload finished.\n";
    return 0;
}
```

编译（保留调试符号，以便火焰图展示函数名）：

```bash
g++ -g -O2 -std=c++11 -o example example.cpp
```

## 2. 生成火焰图的完整流程

### 步骤1：运行程序并用 perf 采集调用栈

```bash
# 以 99Hz 频率采样 CPU 事件，记录调用栈，持续30秒
sudo perf record -F 99 -g -- ./example

# 或者对已运行的进程采样（找到PID后执行）
# sudo perf record -F 99 -g -p <PID> -- sleep 30
```

参数说明：
- `-F 99`：采样频率 99Hz（避免与系统定时器共振）
- `-g`：记录调用图（call graph）
- `-- ./example`：要运行的命令

### 步骤2：生成 perf 原始数据文件

```bash
# 查看采样数据统计
sudo perf report -n --stdio | head -20
```

会看到类似输出：
```
# Samples: 2K of event 'cpu-clock'
# Event count (approx.): 300000000
...
```

### 步骤3：下载 FlameGraph 工具

```bash
git clone https://github.com/brendangregg/FlameGraph
cd FlameGraph
```

### 步骤4：转换 perf 数据为火焰图格式

```bash
# 将 perf.data 转换为可折叠的调用栈格式
sudo perf script > out.perf

# 折叠调用栈
./stackcollapse-perf.pl out.perf > out.folded

# 生成 SVG 火焰图
./flamegraph.pl out.folded > flamegraph.svg
```

### 步骤5（可选）：一键生成脚本

创建 `generate_flamegraph.sh`：

```bash
#!/bin/bash
sudo perf record -F 99 -g -- ./example
sudo perf script > out.perf
./FlameGraph/stackcollapse-perf.pl out.perf > out.folded
./FlameGraph/flamegraph.pl out.folded > flamegraph.svg
echo "Flame graph generated: flamegraph.svg"
```

```bash
chmod +x generate_flamegraph.sh
./generate_flamegraph.sh
```

## 3. 查看火焰图

用浏览器打开 `flamegraph.svg`：

```bash
# Linux 下
xdg-open flamegraph.svg

# macOS 下
open flamegraph.svg

# 或直接用浏览器打开
firefox flamegraph.svg
```

## 4. 预期结果分析

生成的火焰图中你应该能看到：

- **`main`** 在最底部
- **`recursiveSum`** 函数呈较宽的条（因递归深度大）
- **`computeTrig`** 和 **`computeIntegers`** 也占用明显宽度
- 每个条的长度表示该函数在采样中出现的相对频率（CPU时间占比）

## 5. 常见问题及调试

| 问题 | 解决方法 |
|------|----------|
| 符号显示为地址而非函数名 | 编译时加 `-g`，或安装 `debuginfo` 包 |
| 看不到调用栈 | 使用 `sudo perf record -F 99 -g --call-graph dwarf ./example` |
| 采样时间太长/太短 | 调整 `sleep` 时间或 `--` 后程序的运行时长 |
| 需要更精确的栈回溯 | 使用 `--call-graph dwarf,16384` 增加 dwarf 栈大小 |

## 6. 进阶：生成差分火焰图

比较优化前后的性能变化：

```bash
# 优化前采集
sudo perf record -F 99 -g -- ./example_old -o perf_old.data

# 优化后采集
sudo perf record -F 99 -g -- ./example_new -o perf_new.data

# 生成两个折叠文件
sudo perf script -i perf_old.data | ./FlameGraph/stackcollapse-perf.pl > old.folded
sudo perf script -i perf_new.data | ./FlameGraph/stackcollapse-perf.pl > new.folded

# 生成差分火焰图
./FlameGraph/difffolded.pl old.folded new.folded | ./FlameGraph/flamegraph.pl > diff.svg
```

红色表示优化后占比增加，蓝色表示减少，非常直观地展示性能变化。
