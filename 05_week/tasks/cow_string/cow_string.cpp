#include <cstring>
#include <string>

class CowString {
private:
    struct StringData {
        char* data;
        size_t size;
        size_t ref_count;

        StringData(const char* str = "") {
            size = std::strlen(str);
            data = new char[size + 1];
            std::memcpy(data, str, size + 1);
            ref_count = 1;
        }

        ~StringData() {
            delete[] data;
        }
    };

    StringData* m_data;

    void Release() {
        if (m_data && --m_data->ref_count == 0) {
            delete m_data;
        }
    }

    void Detach() {
        if (m_data->ref_count > 1) {
            StringData* newData = new StringData(m_data->data);
            --m_data->ref_count;
            m_data = newData;
        }
    }

public:
    static inline const size_t npos = static_cast<size_t>(-1);

    // ---- Constructors ----

    CowString()
        : m_data(new StringData("")) {}

    CowString(const char* str)
        : m_data(new StringData(str ? str : "")) {}

    CowString(const std::string& str)
        : m_data(new StringData(str.c_str())) {}

    CowString(const CowString& other)
        : m_data(other.m_data) {
        ++m_data->ref_count;
    }

    CowString(CowString&& other) noexcept
        : m_data(other.m_data) {
        other.m_data = nullptr;
    }

    // ---- Assignment ----

    CowString& operator=(const CowString& other) {
        if (this != &other) {
            Release();
            m_data = other.m_data;
            ++m_data->ref_count;
        }
        return *this;
    }

    CowString& operator=(CowString&& other) noexcept {
        if (this != &other) {
            Release();
            m_data = other.m_data;
            other.m_data = nullptr;
        }
        return *this;
    }

    // ---- Destructor ----

    ~CowString() {
        Release();
    }

    // ---- Non-modifying ----

    size_t Size() const {
        return m_data ? m_data->size : 0;
    }

    bool Empty() const {
        return Size() == 0;
    }

    const char* ToCstr() const {
        return m_data ? m_data->data : "";
    }

    std::string ToString() const {
        return std::string(ToCstr());
    }

    operator const char*() const {
        return ToCstr();
    }

    const char& operator[](size_t index) const {
        return m_data->data[index];
    }

    // ---- Modifying (COW) ----

    char& operator[](size_t index) {
        Detach();
        return m_data->data[index];
    }

    CowString& Append(const char* str) {
        if (!str) return *this;

        size_t addSize = std::strlen(str);
        if (addSize == 0) {
            return *this;   // ВАЖНО: без Detach()
        }

        Detach();

        size_t newSize = m_data->size + addSize;
        char* newBuffer = new char[newSize + 1];

        std::memcpy(newBuffer, m_data->data, m_data->size);
        std::memcpy(newBuffer + m_data->size, str, addSize + 1);

        delete[] m_data->data;

        m_data->data = newBuffer;
        m_data->size = newSize;

        return *this;
    }

    CowString& Append(const std::string& str) {
        return Append(str.c_str());
    }

    CowString Substr(size_t pos = 0, size_t count = npos) const {
        if (pos >= Size()) {
            return CowString("");
        }

        if (count == npos || pos + count > Size()) {
            count = Size() - pos;
        }

        char* buffer = new char[count + 1];
        std::memcpy(buffer, m_data->data + pos, count);
        buffer[count] = '\0';

        CowString result(buffer);
        delete[] buffer;

        return result;
    }

    void Clear() {
        Detach();

        delete[] m_data->data;
        m_data->data = new char[1];
        m_data->data[0] = '\0';
        m_data->size = 0;
    }

    // ---- Find ----

    size_t Find(const char* str) const {
        if (!str) return npos;

        size_t len = std::strlen(str);
        if (len == 0) return 0;
        if (len > Size()) return npos;

        for (size_t i = 0; i <= Size() - len; ++i) {
            if (std::memcmp(m_data->data + i, str, len) == 0) {
                return i;
            }
        }

        return npos;
    }

    size_t Find(char ch) const {
        for (size_t i = 0; i < Size(); ++i) {
            if (m_data->data[i] == ch) {
                return i;
            }
        }
        return npos;
    }
};


