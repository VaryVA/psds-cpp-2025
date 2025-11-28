#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>

enum class CheckFlags : uint8_t {
    NONE = 0,
    TIME = (1 << 0),
    DATE = (1 << 1),
    USER = (1 << 2),
    CERT = (1 << 3),
    KEYS = (1 << 4),
    DEST = (1 << 5),
    ALL = TIME | DATE | USER | CERT | KEYS | DEST
};

void PrintCheckFlags(CheckFlags flags) {
    uint8_t value = static_cast<uint8_t>(flags);
    
    if (value > static_cast<uint8_t>(CheckFlags::ALL)) {
        return;
    }
    
    std::string result = "[";
    bool first = true;
    
    const std::pair<CheckFlags, const char*> flagNames[] = {
        {CheckFlags::TIME, "TIME"},
        {CheckFlags::DATE, "DATE"},
        {CheckFlags::USER, "USER"},
        {CheckFlags::CERT, "CERT"},
        {CheckFlags::KEYS, "KEYS"},
        {CheckFlags::DEST, "DEST"}
    };
    
    for (const auto& [flag, name] : flagNames) {
        if (value & static_cast<uint8_t>(flag)) {
            if (!first) result += ",";
            result += name;
            first = false;
        }
    }
    
    result += "]";
    std::cout << result;
}
