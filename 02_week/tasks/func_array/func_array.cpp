#include <stdexcept>

typedef double (*MathOperation)(double, double);

double ApplyOperations(double a, double b, MathOperation operations[], size_t size) {
    if (operations == nullptr || size == 0) return 0.0;
    
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) sum += operations[i] ? operations[i](a, b) : 0.0;
    
    return sum;
}
