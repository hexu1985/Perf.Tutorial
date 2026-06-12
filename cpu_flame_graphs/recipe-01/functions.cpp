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

