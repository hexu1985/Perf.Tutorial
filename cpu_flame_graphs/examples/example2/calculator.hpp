#pragma once

#include <vector>

namespace Math {

class Calculator {
public:
    double compute(std::vector<int>& data);
    
private:
    double process(int value); 
};

}   // namespace Math

