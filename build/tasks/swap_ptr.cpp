#include <stdexcept>

template<typename T>
void SwapPtr(T& ptr1, T& ptr2) {
    T temp = ptr1;
    ptr1 = ptr2;
    ptr2 = temp;
}
