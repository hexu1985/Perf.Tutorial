#include "calculator.hpp"

namespace Math {

double Calculator::compute(std::vector<int>& data) {
    double sum = 0;
    for (int x : data) {
        sum += process(x);
    }
    return sum / data.size();
}
    
double Calculator::process(int value) {
    return value * 1.5;
}

}   // namespace Math
