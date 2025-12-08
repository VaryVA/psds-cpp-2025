#include <stdexcept>

template<typename CharPtr>
CharPtr FindLongestSubsequenceImpl(CharPtr begin, CharPtr end, size_t& count) {
    if (!begin || !end || begin >= end) {
        count = 0;
        return nullptr;
    }
    
    CharPtr best = begin;
    CharPtr current = begin;
    size_t bestLen = 1;
    size_t currentLen = 1;
    
    for (CharPtr p = begin + 1; p < end; ++p) {
        (*p == *(p - 1)) ? 
            (currentLen++, (currentLen > bestLen) ? (void)(bestLen = currentLen, best = current) : (void)0) :
            (void)(current = p, currentLen = 1);
    }
    
    count = bestLen;
    return best;
}

char* FindLongestSubsequence(char* begin, char* end, size_t& count) {
    return FindLongestSubsequenceImpl(begin, end, count);
}

const char* FindLongestSubsequence(const char* begin, const char* end, size_t& count) {
    return FindLongestSubsequenceImpl(begin, end, count);
}
