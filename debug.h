#include <iostream>

namespace {
    template<typename T>
    struct has_const_iterator {
    private:
        typedef char                        yes;
        typedef struct {char array[2];}     no;
        template<typename C> static yes test(typename C::const_iterator*);
        template<typename C> static no test(...);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(yes);
        typedef T type;
    };

    template<typename T>
    struct has_begin_end {
        template<typename C> static char (&f(typename std::enable_if<
            std::is_same<decltype(static_cast<typename C::const_iterator
            (C::*)() const>(&C::begin)), typename C::const_iterator(C::*)() const>
            ::value, void>::type*))[1];

        template<typename C> static char (&f(...))[2];

        template<typename C> static char (&g(typename std::enable_if<
            std::is_same<decltype(static_cast<typename C::const_iterator
            (C::*)() const>(&C::end)), typename C::const_iterator(C::*)() const>::value,
            void>::type*))[1];

        template<typename C> static char (&g(...))[2];

        static bool const begin_value = sizeof(f<T>(0)) == 1;
        static bool const end_value = sizeof(g<T>(0)) == 1;
    };

    template<typename T>
    struct is_container :
        std::integral_constant<bool, has_const_iterator<T>::value
            && has_begin_end<T>::begin_value && has_begin_end<T>::end_value>
            {};
        template<typename T, typename=typename std::enable_if<is_container<T>::value>::type>
    
    inline std::ostream& operator<<(std::ostream& out, const T& container) {
        for (const auto& value : container) {
            std::cout << value << " ";
        }
        return out;
    }
}

template<typename T>
static void Log(const T& value) {
    std::cout << value << std::endl;
}

namespace {
    template<typename T>
    static void assert(const T& first, const T& second, 
        const char* file_name = __FILE__, int line = __LINE__) {
            if (first == second) return;
            std::cerr << "\033[1;31mAssertion failed: FILE: " << file_name << " LINE: " 
                << line << "\033[0m";
    }
}

#define Assert(first, second) assert(first, second, __FILE__, __LINE__)
