#include <string>
#include <algorithm>

class SimpleList {
private:
    // Структура узла списка
    struct Node {
        std::string value;
        Node* next;
        Node* prev;
        
        explicit Node(const std::string& val) : value(val), next(nullptr), prev(nullptr) {}
        explicit Node(std::string&& val) : value(std::move(val)), next(nullptr), prev(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t count;
    
    void Unlink(Node* node) {
        if (!node) return;
        
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        
        delete node;
        --count;
    }
    
    void LinkBefore(Node* newNode, Node* beforeNode) {
        if (!beforeNode) {
            if (!head) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        } else {
            newNode->prev = beforeNode->prev;
            newNode->next = beforeNode;
            
            if (beforeNode->prev) {
                beforeNode->prev->next = newNode;
            } else {
                head = newNode;
            }
            
            beforeNode->prev = newNode;
        }
        
        ++count;
    }

public:
    SimpleList() : head(nullptr), tail(nullptr), count(0) {}
    
    SimpleList(const SimpleList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* current = other.head;
        while (current) {
            PushBack(current->value);
            current = current->next;
        }
    }
    
    SimpleList(SimpleList&& other) noexcept 
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }
    
    ~SimpleList() {
        Clear();
    }
    
    SimpleList& operator=(const SimpleList& other) {
        if (this != &other) {
            SimpleList temp(other);
            Swap(temp);
        }
        return *this;
    }
    
    SimpleList& operator=(SimpleList&& other) noexcept {
        if (this != &other) {
            Clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            
            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }
    
    void Swap(SimpleList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }
    
    size_t Size() const {
        return count;
    }
    
    bool Empty() const {
        return count == 0;
    }
    
    void PushBack(const std::string& value) {
        LinkBefore(new Node(value), nullptr);
    }
    
    void PushBack(std::string&& value) {
        LinkBefore(new Node(std::move(value)), nullptr);
    }
    
    void PopBack() {
        if (tail) {
            Unlink(tail);
        }
    }
    
    void PushFront(const std::string& value) {
        LinkBefore(new Node(value), head);
    }
    
    void PushFront(std::string&& value) {
        LinkBefore(new Node(std::move(value)), head);
    }
    
    void PopFront() {
        if (head) {
            Unlink(head);
        }
    }
    
    std::string& Back() {
        return tail->value;
    }

    const std::string& Back() const {
        return tail->value;
    }
    
    std::string& Front() {
        return head->value;
    }
    
    const std::string& Front() const {
        return head->value;
    }
    void Clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }
};

inline void Swap(SimpleList& lhs, SimpleList& rhs) noexcept {
    lhs.Swap(rhs);
}
