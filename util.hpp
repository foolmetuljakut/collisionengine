#ifndef spheco_util_hpp
#define spheco_util_hpp

#include <exception>
#include <string>
#include <cstddef>

namespace spheco {
    class Exception : public std::exception {
        const char* m;
    public:
        Exception(const std::string& msg) : m{msg.c_str()} {}
        virtual const char* what() const noexcept { return m; }
    };

    class NullPtrException : public Exception {};

    class BranchLeafException : public Exception {
    public:
        float branchvalue;
        size_t depth;
    };
}

namespace std { 
    template <typename Iter, class T>
    size_t index_of(Iter first, Iter last, const T& x)
    {
        size_t i = 0;
        while (first != last && *first != x)
            ++first, ++i;
        return i;
    }
}

#endif //spheco_util_hpp



