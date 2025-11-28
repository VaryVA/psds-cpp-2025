#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <stdexcept>

void SolveQuadratic(int a, int b, int c) {
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                std::cout << "infinite solutions";
            } 
            else {
                std::cout << "no solutions";
            }
        } 
        else {
            double x = -static_cast<double>(c) / b;
            if (x == 0.0) x = 0.0;
            std::cout << std::setprecision(6) << x;
        }
        return;
    }
    
    double discriminant = static_cast<double>(b) * b - 4.0 * a * c;

    if (discriminant < 0) {
        std::cout << "no solutions";
    }
    else if (discriminant == 0) {
        double x = -static_cast<double>(b) / (2.0 * a);
        if (x == 0.0) x = 0.0;
        std::cout << std::setprecision(6) << x;
    }
    else {
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-static_cast<double>(b) - sqrt_d) / (2.0 * a);
        double x2 = (-static_cast<double>(b) + sqrt_d) / (2.0 * a);
        
        if (x1 == 0.0) x1 = 0.0;
        if (x2 == 0.0) x2 = 0.0;
        
        if (x1 > x2) {
            std::swap(x1, x2);
        }
        
        std::cout << std::setprecision(6) << x1 << " " << x2;
    }
}
