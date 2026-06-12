// example.cpp
#include <vector>
#include <iostream>

#include "calculator.hpp"

int main() {
    Math::Calculator calc;
    std::vector<int> data = {1, 2, 3, 4, 5};
    
    double sum = 0;
    while (true) {
        double result = calc.compute(data);
        sum += result;
    }

    (void) sum;
    return 0;
}
