#include <string>
#include <utility>

class SharedPtr;
class WeakPtr;

struct ControlBlock {
    std::string* data;
    size_t strong_count;
    size_t weak_count;

    ControlBlock(std::string* ptr) : data(ptr), strong_count(1), weak_count(0) {}
    
    void add_strong() { ++strong_count; }
    void release_strong() {
        if (--strong_count == 0) {
            delete data;
            data = nullptr;
            if (weak_count == 0) {
                delete this;
            }
        }
    }
    
    void add_weak() { ++weak_count; }
    void release_weak() {
        if (--weak_count == 0 && strong_count == 0) {
            delete this;
        }
    }
};

class SharedPtr {
private:
    std::string* ptr_;
    ControlBlock* control_;

    friend class WeakPtr;

public:
    SharedPtr() : ptr_(nullptr), control_(nullptr) {}

    explicit SharedPtr(std::string* ptr) {
        if (ptr) {
            control_ = new ControlBlock(ptr);
            ptr_ = ptr;
        } else {
            ptr_ = nullptr;
            control_ = nullptr;
        }
    }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), control_(other.control_) {
        if (control_) {
            control_->add_strong();
        }
    }

    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), control_(other.control_) {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    explicit SharedPtr(const WeakPtr& weak);

    ~SharedPtr() {
        if (control_) {
            control_->release_strong();
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            if (other.control_) {
                other.control_->add_strong();
            }
            if (control_) {
                control_->release_strong();
            }
            ptr_ = other.ptr_;
            control_ = other.control_;
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            if (control_) {
                control_->release_strong();
            }
            ptr_ = other.ptr_;
            control_ = other.control_;
            other.ptr_ = nullptr;
            other.control_ = nullptr;
        }
        return *this;
    }

    std::string& operator*() const {
        return *ptr_;
    }

    std::string* operator->() const {
        return ptr_;
    }

    std::string* Get() const {
        return ptr_;
    }

    void Reset(std::string* ptr = nullptr) {
        SharedPtr(ptr).Swap(*this);
    }

    void Swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(control_, other.control_);
    }

    size_t UseCount() const {
        return control_ ? control_->strong_count : 0;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }
};

class WeakPtr {
private:
    std::string* ptr_;
    ControlBlock* control_;

    friend class SharedPtr;

public:
    WeakPtr() : ptr_(nullptr), control_(nullptr) {}

    WeakPtr(const SharedPtr& shared) : ptr_(shared.ptr_), control_(shared.control_) {
        if (control_) {
            control_->add_weak();
        }
    }

    WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), control_(other.control_) {
        if (control_) {
            control_->add_weak();
        }
    }

    WeakPtr(WeakPtr&& other) noexcept 
        : ptr_(other.ptr_), control_(other.control_) {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    ~WeakPtr() {
        if (control_) {
            control_->release_weak();
        }
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            if (other.control_) {
                other.control_->add_weak();
            }
            if (control_) {
                control_->release_weak();
            }
            ptr_ = other.ptr_;
            control_ = other.control_;
        }
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            if (control_) {
                control_->release_weak();
            }
            ptr_ = other.ptr_;
            control_ = other.control_;
            other.ptr_ = nullptr;
            other.control_ = nullptr;
        }
        return *this;
    }

    WeakPtr& operator=(const SharedPtr& shared) {
        if (shared.control_) {
            shared.control_->add_weak();
        }
        if (control_) {
            control_->release_weak();
        }
        ptr_ = shared.ptr_;
        control_ = shared.control_;
        return *this;
    }

    void Reset() {
        WeakPtr().Swap(*this);
    }

    void Swap(WeakPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(control_, other.control_);
    }

    size_t UseCount() const {
        return control_ ? control_->strong_count : 0;
    }

    bool Expired() const {
        return !control_ || control_->strong_count == 0;
    }

    SharedPtr Lock() const;
};

inline SharedPtr::SharedPtr(const WeakPtr& weak) : ptr_(weak.ptr_), control_(weak.control_) {
    if (control_) {
        if (control_->strong_count > 0) {
            control_->add_strong();
        } else {
            ptr_ = nullptr;
            control_ = nullptr;
        }
    }
}

// Определение Lock для WeakPtr
inline SharedPtr WeakPtr::Lock() const {
    if (control_ && control_->strong_count > 0) {
        return SharedPtr(*this);
    }
    return SharedPtr();
}

inline SharedPtr MakeShared(const std::string& str) {
    return SharedPtr(new std::string(str));
}

inline SharedPtr MakeShared(std::string&& str) {
    return SharedPtr(new std::string(std::move(str)));
}

inline void Swap(SharedPtr& lhs, SharedPtr& rhs) noexcept {
    lhs.Swap(rhs);
}

inline void Swap(WeakPtr& lhs, WeakPtr& rhs) noexcept {
    lhs.Swap(rhs);
}
