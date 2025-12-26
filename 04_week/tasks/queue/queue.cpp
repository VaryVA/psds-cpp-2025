#include <vector>
#include <stack>
#include <initializer_list>
#include <cstddef>
#include <utility>

class Queue {
public:
    Queue() = default;

    explicit Queue(std::size_t capacity) {
        in_.reserve(capacity);
    }

    explicit Queue(const std::vector<int>& v) {
        in_ = v;
    }

    Queue(std::initializer_list<int> il) {
        in_.assign(il.begin(), il.end());
    }

    explicit Queue(std::stack<int> st) {
        std::vector<int> tmp;
        while (!st.empty()) {
            tmp.push_back(st.top());
            st.pop();
        }
        for (auto it = tmp.rbegin(); it != tmp.rend(); ++it) {
            in_.push_back(*it);
        }
    }

    void Push(int value) {
        in_.push_back(value);
    }

    bool Pop() {
        if (Empty()) {
            return false;
        }
        MoveIfNeeded();
        out_.pop_back();
        return true;
    }

    int& Front() {
        MoveIfNeeded();
        return out_.back();
    }

    const int& Front() const {
        MoveIfNeeded();
        return out_.back();
    }

    int& Back() {
        if (!in_.empty()) {
            return in_.back();
        }
        return out_.front();
    }

    const int& Back() const {
        if (!in_.empty()) {
            return in_.back();
        }
        return out_.front();
    }

    bool Empty() const {
        return in_.empty() && out_.empty();
    }

    std::size_t Size() const {
        return in_.size() + out_.size();
    }

    void Clear() {
        in_.clear();
        out_.clear();
    }

    void Swap(Queue& other) {
        in_.swap(other.in_);
        out_.swap(other.out_);
    }

    friend bool operator==(const Queue& a, const Queue& b) {
        if (a.Size() != b.Size()) {
            return false;
        }
        return a.ToVector() == b.ToVector();
    }

    friend bool operator!=(const Queue& a, const Queue& b) {
        return !(a == b);
    }

private:
    std::vector<int> in_;
    std::vector<int> out_;

    void MoveIfNeeded() const {
        if (!out_.empty()) {
            return;
        }

        Queue* self = const_cast<Queue*>(this);
        while (!self->in_.empty()) {
            self->out_.push_back(self->in_.back());
            self->in_.pop_back();
        }
    }

    std::vector<int> ToVector() const {
        std::vector<int> res;
        res.reserve(Size());

        for (auto it = out_.rbegin(); it != out_.rend(); ++it) {
            res.push_back(*it);
        }

        for (int x : in_) {
            res.push_back(x);
        }

        return res;
    }
};

