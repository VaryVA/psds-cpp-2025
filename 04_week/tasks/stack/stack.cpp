#include <vector>

class Stack {
private:
    std::vector<int> data;

public:
    Stack() = default;
    
    void Push(int value) {
        data.push_back(value);
    }
    
    bool Pop() {
        if (Empty()) {
            return false;
        }
        data.pop_back();
        return true;
    }
    
    int& Top() {
        return data.back();
    }
    
    const int& Top() const {
        return data.back();
    }
    
    bool Empty() const {
        return data.empty();
    }
    
    size_t Size() const {
        return data.size();
    }
    
    void Clear() {
        data.clear();
    }
    
    void Swap(Stack& other) {
        data.swap(other.data);
    }
    
    bool operator==(const Stack& other) const {
        return data == other.data;
    }
    
    bool operator!=(const Stack& other) const {
        return data != other.data;
    }
};
