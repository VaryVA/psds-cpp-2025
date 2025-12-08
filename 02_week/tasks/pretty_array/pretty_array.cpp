#include <iostream>
#include <stdexcept>

void PrintArray(const int* begin, const int* end, int limit = 0) {
    if (begin == nullptr || end == nullptr) {
        std::cout << "[]\n";
        return;
    }
    
    if (begin == end) {
        std::cout << "[]\n";
        return;
    }
    
    bool reverse = (end < begin);
    
    int count = 0;
    if (!reverse) {
        count = static_cast<int>(end - begin);
    } else {
        count = static_cast<int>(begin - end);
    }
    
    if (count == 0) {
        std::cout << "[]\n";
        return;
    }
    
    std::cout << "[";
    
    int elements_in_current_line = 0;
    bool first_element = true;
    
    for (int i = 0; i < count; i++) {
        const int* current;
        if (!reverse) {
            current = begin + i;
        } else {
            current = begin - i;
        }
        
        if (!first_element) {
            std::cout << ", ";
        }
        
        std::cout << *current;
        elements_in_current_line++;
        
        if (limit > 0 && elements_in_current_line >= limit && i < count - 1) {
            std::cout << ", ...\n ";
            elements_in_current_line = 0;
            first_element = true;
        } else {
            first_element = false;
        }
    }
    
    std::cout << "]\n";
}
