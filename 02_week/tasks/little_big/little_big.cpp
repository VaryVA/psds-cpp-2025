#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>

template<typename T>
void PrintMemoryImpl(T value, bool reverseBytes) {
    unsigned char bytes[sizeof(T)];
    std::memcpy(bytes, &value, sizeof(T));
    
    std::cout << "0x";
    
    if (reverseBytes) {
        for (int i = sizeof(T) - 1; i >= 0; --i) {
            std::cout << std::hex << std::setfill('0') << std::setw(2) 
                      << std::uppercase << static_cast<int>(bytes[i]);
        }
    } else {
        for (size_t i = 0; i < sizeof(T); ++i) {
            std::cout << std::hex << std::setfill('0') << std::setw(2) 
                      << std::uppercase << static_cast<int>(bytes[i]);
        }
    }
    
    std::cout << std::dec << std::endl;
}

void PrintMemory(int value, bool reverseBytes = false) {
    PrintMemoryImpl(value, reverseBytes);
}

void PrintMemory(double value, bool reverseBytes = false) {
    PrintMemoryImpl(value, reverseBytes);
}
