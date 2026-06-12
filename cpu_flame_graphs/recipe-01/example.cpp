#include <iostream>
#include "functions.hpp"

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
