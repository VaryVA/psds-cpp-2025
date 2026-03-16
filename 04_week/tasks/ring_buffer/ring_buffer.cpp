#include <vector>
#include <algorithm>
#include <cassert>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) 
        : buffer_(capacity > 0 ? capacity : 1), 
          head_(0), 
          tail_(0), 
          size_(0) {}
    
    RingBuffer(size_t capacity, int value) 
        : buffer_(capacity > 0 ? capacity : 1, value),
          head_(0),
          tail_(0),
          size_(capacity > 0 ? capacity : 1) {
        tail_ = size_ % buffer_.size();
    }
    
    RingBuffer(std::initializer_list<int> init) 
        : buffer_(init.size() > 0 ? init.size() : 1),
          head_(0),
          tail_(init.size() % buffer_.size()),
          size_(init.size()) {
        std::copy(init.begin(), init.end(), buffer_.begin());
    }
    
    void Push(int value) {
        buffer_[tail_] = value;
        if (size_ < buffer_.size()) {
            ++size_;
        } else {
            head_ = (head_ + 1) % buffer_.size();
        }
        tail_ = (tail_ + 1) % buffer_.size();
    }
    
    bool TryPush(int value) {
        if (size_ == buffer_.size()) {
            return false;
        }
        Push(value);
        return true;
    }
    
    void Pop() {
        if (size_ > 0) {
            head_ = (head_ + 1) % buffer_.size();
            --size_;
        }
    }
    
    bool TryPop(int& value) {
        if (size_ == 0) {
            return false;
        }
        value = buffer_[head_];
        Pop();
        return true;
    }
    
    int& operator[](size_t index) {
        assert(index < size_ && "Index out of bounds");
        return buffer_[(head_ + index) % buffer_.size()];
    }
    
    const int& operator[](size_t index) const {
        assert(index < size_ && "Index out of bounds");
        return buffer_[(head_ + index) % buffer_.size()];
    }
    
    int& Front() {
        assert(size_ > 0 && "Buffer is empty");
        return buffer_[(tail_ == 0 ? buffer_.size() - 1 : tail_ - 1)];
    }
    
    const int& Front() const {
        assert(size_ > 0 && "Buffer is empty");
        return buffer_[(tail_ == 0 ? buffer_.size() - 1 : tail_ - 1)];
    }
    
    int& Back() {
        assert(size_ > 0 && "Buffer is empty");
        return buffer_[head_];
    }
    
    const int& Back() const {
        assert(size_ > 0 && "Buffer is empty");
        return buffer_[head_];
    }
    
    bool Empty() const {
        return size_ == 0;
    }
    
    bool Full() const {
        return size_ == buffer_.size();
    }
    
    size_t Size() const {
        return size_;
    }
    
    size_t Capacity() const {
        return buffer_.size();
    }
    
    void Clear() {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }
    
    void Resize(size_t new_capacity) {
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        
        std::vector<int> new_buffer(new_capacity);
        size_t new_size = std::min(size_, new_capacity);
        
        if (new_size < size_) {
            size_t start_index = size_ - new_size;
            for (size_t i = 0; i < new_size; ++i) {
                new_buffer[i] = (*this)[start_index + i];
            }
            head_ = 0;
            tail_ = new_size % new_capacity;
        } else {
            for (size_t i = 0; i < new_size; ++i) {
                new_buffer[i] = (*this)[i];
            }
            head_ = 0;
            tail_ = new_size % new_capacity;
        }
        
        buffer_.swap(new_buffer);
        size_ = new_size;
    }
    
    std::vector<int> Vector() const {
        std::vector<int> result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = (*this)[i];
        }
        return result;
    }

private:
    std::vector<int> buffer_;
    size_t head_;
    size_t tail_;
    size_t size_;
};
