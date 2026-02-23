#include <string>
#include <utility>

class UniquePtr {
private:
    std::string* ptr;

public:
    UniquePtr() noexcept : ptr(nullptr) {}
    explicit UniquePtr(std::string* p) noexcept : ptr(p) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
    ~UniquePtr() { delete ptr; }

    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    std::string& operator*() const { return *ptr; }
    std::string* operator->() const noexcept { return ptr; }

    std::string* Get() const noexcept { return ptr; }
    std::string* Release() noexcept { std::string* tmp = ptr; ptr = nullptr; return tmp; }
    void Reset(std::string* p = nullptr) noexcept { delete ptr; ptr = p; }
    void Swap(UniquePtr& other) noexcept { std::swap(ptr, other.ptr); }
    explicit operator bool() const noexcept { return ptr != nullptr; }
};

inline UniquePtr MakeUnique(const std::string& str) {
    return UniquePtr(new std::string(str));
}
inline UniquePtr MakeUnique(std::string&& str) {
    return UniquePtr(new std::string(std::move(str)));
}

inline void Swap(UniquePtr& a, UniquePtr& b) noexcept {
    a.Swap(b);
}
