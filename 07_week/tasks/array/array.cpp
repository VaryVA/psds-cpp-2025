#include <initializer_list>
#include <algorithm>
#include <stdexcept>

template<typename T, size_t N>
class Array {
private:
    T data_[N];

public:
    // Конструктор по умолчанию
    Array() = default;

    // Конструктор, принимающий список инициализации
    Array(std::initializer_list<T> list) {
        size_t i = 0;
        for (const auto& item : list) {
            if (i < N) {
                data_[i++] = item;
            }
        }
        // Если элементов в списке меньше N, остальные будут value-инициализированы
    }

    // Конструктор копирования
    Array(const Array& other) = default;

    // Конструктор перемещения
    Array(Array&& other) noexcept = default;

    // Деструктор
    ~Array() = default;

    // Оператор присваивания копированием
    Array& operator=(const Array& other) = default;

    // Оператор присваивания перемещением
    Array& operator=(Array&& other) noexcept = default;

    // Операторы индексирования
    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    // Метод Front
    T& Front() {
        return data_[0];
    }

    const T& Front() const {
        return data_[0];
    }

    // Метод Back
    T& Back() {
        return data_[N - 1];
    }

    const T& Back() const {
        return data_[N - 1];
    }

    // Метод Data
    T* Data() {
        return data_;
    }

    const T* Data() const {
        return data_;
    }

    // Метод Empty
    constexpr bool Empty() const {
        return N == 0;
    }

    // Метод Size
    constexpr size_t Size() const {
        return N;
    }

    // Метод Fill
    void Fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = value;
        }
    }

    // Метод Swap
    void Swap(Array& other) noexcept {
        for (size_t i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

    // Методы для range-based for
    T* begin() {
        return data_;
    }

    const T* begin() const {
        return data_;
    }

    T* end() {
        return data_ + N;
    }

    const T* end() const {
        return data_ + N;
    }

    // Константные итераторы
    const T* cbegin() const {
        return data_;
    }

    const T* cend() const {
        return data_ + N;
    }
};

// Внешние функции

// Операторы сравнения
template<typename T, size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

template<typename T, size_t N>
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs == rhs);
}

template<typename T, size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] < rhs[i]) return true;
        if (rhs[i] < lhs[i]) return false;
    }
    return false;
}

template<typename T, size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return rhs < lhs;
}

template<typename T, size_t N>
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(rhs < lhs);
}

template<typename T, size_t N>
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs < rhs);
}

// Функция swap
template<typename T, size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept {
    lhs.Swap(rhs);
}

// Функция get для доступа по индексу (аналогично кортежу)
template<size_t I, typename T, size_t N>
T& get(Array<T, N>& arr) {
    static_assert(I < N, "Index out of bounds");
    return arr[I];
}

template<size_t I, typename T, size_t N>
const T& get(const Array<T, N>& arr) {
    static_assert(I < N, "Index out of bounds");
    return arr[I];
}

template<size_t I, typename T, size_t N>
T&& get(Array<T, N>&& arr) {
    static_assert(I < N, "Index out of bounds");
    return std::move(arr[I]);
}
