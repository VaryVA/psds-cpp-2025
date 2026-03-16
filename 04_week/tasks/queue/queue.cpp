#include <vector>
#include <stack>
#include <initializer_list>
#include <algorithm>

class Queue {
private:
    std::vector<int> input_stack;
    std::vector<int> output_stack;
    
    void transfer_elements() {
        if (output_stack.empty()) {
            while (!input_stack.empty()) {
                output_stack.push_back(input_stack.back());
                input_stack.pop_back();
            }
        }
    }

public:
    Queue() = default;
    
    Queue(std::stack<int> s) {
        while (!s.empty()) {
            input_stack.push_back(s.top());
            s.pop();
        }
        std::reverse(input_stack.begin(), input_stack.end());
    }
    
    Queue(const std::vector<int>& vec) : input_stack(vec) {}
    
    Queue(std::initializer_list<int> init) : input_stack(init) {}
    
    explicit Queue(size_t size) {
        input_stack.reserve(size);
        output_stack.reserve(size);
    }
    
    Queue(const Queue& other) 
        : input_stack(other.input_stack), output_stack(other.output_stack) {}
    
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            input_stack = other.input_stack;
            output_stack = other.output_stack;
        }
        return *this;
    }
    
    ~Queue() = default;
    
    void Push(int value) {
        input_stack.push_back(value);
    }
    
    bool Pop() {
        if (Empty()) {
            return false;
        }
        
        transfer_elements();
        if (!output_stack.empty()) {
            output_stack.pop_back();
            return true;
        }
        return false;
    }
    
    int& Front() {
        transfer_elements();
        return output_stack.back();
    }
    
    const int& Front() const {
        if (!output_stack.empty()) {
            return output_stack.back();
        }
        return input_stack.front();
    }
    
    int& Back() {
        if (!input_stack.empty()) {
            return input_stack.back();
        }
        return output_stack.front();
    }
    
    const int& Back() const {
        if (!input_stack.empty()) {
            return input_stack.back();
        }
        return output_stack.front();
    }
    
    bool Empty() const {
        return input_stack.empty() && output_stack.empty();
    }
    
    size_t Size() const {
        return input_stack.size() + output_stack.size();
    }
    
    void Clear() {
        input_stack.clear();
        output_stack.clear();
    }
    
    void Swap(Queue& other) {
        input_stack.swap(other.input_stack);
        output_stack.swap(other.output_stack);
    }
    
    bool operator==(const Queue& other) const {
        if (Size() != other.Size()) {
            return false;
        }
        
        Queue temp1 = *this;
        Queue temp2 = other;
        
        while (!temp1.Empty()) {
            if (temp1.Front() != temp2.Front()) {
                return false;
            }
            temp1.Pop();
            temp2.Pop();
        }
        return true;
    }
    
    bool operator!=(const Queue& other) const {
        return !(*this == other);
    }
};
