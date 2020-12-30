#ifndef NUM_HPP
#define NUM_HPP

#include <vector>
#include <iostream>
#include <iostream>
#include <concepts>
#include <ranges>

#define BITSIZEOF(T) sizeof(T) * __CHAR_BIT__

#define APPLY_TO_ARITHMETIC_OPERATORS(f)\
f(&) \
f(|) \
f(^) \
f(+) \
f(-) \
f(*) \
f(/) \
f(%) \
f(<<) \
f(>>)

#define APPLY_TO_ARITHMETIC_ASSIGNMENT_OPERATORS(f)\
f(&=) \
f(|=) \
f(^=) \
f(+=) \
f(-=) \
f(*=) \
f(/=) \
f(%=) \
f(<<=) \
f(>>=)


struct large_num;

std::ostream &operator<<(std::ostream &os, const large_num &n);
std::istream &operator>>(std::istream &is, const large_num &n);

#define PRIVATE_ACCESS 0

#if PRIVATE_ACCESS
    #define private public
#endif

// // helper function
// constexpr size_t divide_round_up(size_t a, size_t b) noexcept{
//     return a / b + static_cast<bool>(a % b);
// }

struct large_num{
    using byte_type = unsigned char; // want: change to uint_fast8_t [or not]
    static_assert(sizeof(byte_type) == 1 && std::is_unsigned_v<byte_type>);
    static constexpr size_t bits_per_byte = BITSIZEOF(byte_type);

    public:
        large_num() = default;
        large_num(const large_num&) = default;
        large_num(large_num&&) = default;
        large_num &operator=(const large_num&) = default;
        large_num &operator=(large_num&&) = default;

        large_num(bool value);

        template<std::integral T>
        large_num(T value){
            constexpr bool may_need_sign_bit = std::is_unsigned_v<T>;
            constexpr size_t storage_size = sizeof(T) + may_need_sign_bit;
            storage = std::vector<byte_type>(storage_size, 0);

            for (auto &byte : storage){
                byte = value;
                value >>= bits_per_byte;
            }

            clean_up();
        }


        large_num operator~() const;
        large_num operator&(const large_num &other) const;
        large_num operator|(const large_num &other) const;
        large_num operator^(const large_num &other) const;

        large_num operator+() const;
        large_num operator-() const;

        large_num &operator++();
        large_num operator++(int);
        large_num &operator--();
        large_num operator--(int);

        large_num operator+(const large_num &other) const;
        large_num operator-(const large_num &other) const;
        large_num operator*(const large_num &other) const;
        large_num operator/(const large_num &other) const;
        large_num operator%(const large_num &other) const;

        large_num operator<<(const large_num &other) const;
        large_num operator>>(const large_num &other) const;

        large_num &operator&=(const large_num &other);
        large_num &operator|=(const large_num &other);
        large_num &operator^=(const large_num &other);

        large_num &operator+=(const large_num &other);
        large_num &operator-=(const large_num &other);
        large_num &operator*=(const large_num &other);
        large_num &operator/=(const large_num &other);
        large_num &operator%=(const large_num &other);

        large_num &operator<<=(const large_num &other);
        large_num &operator>>=(const large_num &other);

        std::strong_ordering operator<=>(const large_num &other) const;

        bool operator==(const large_num &other) const = default;

        operator bool() const;

        template<std::integral T>
        operator T() const{
            T value = is_negative() ? -1 : 0; // fill with sign bits
            constexpr size_t bytes_to_read = sizeof(T);

            for (auto byte : storage | std::views::take(bytes_to_read) | std::views::reverse){
                value <<= bits_per_byte;
                value |= byte;
            }
            
            return value;
        }

        friend std::ostream &operator<<(std::ostream &os, const large_num &n);
        friend std::istream &operator>>(std::istream &is, const large_num &n);
    private:
        void expand_upto(size_t n);
        // enum class initialise_by_size;
        // static initialise_by_size by_size;
        // large_num(const size_t bitsize, const initialise_by_size tag);
        bool get_bit(size_t n) const;
        void set_bit(size_t n, bool value);
        bool sign_bit() const;
        bool is_negative() const;
        bool is_zero() const;
        bool is_positive() const;
        void clean_up();
    private:
        std::vector<byte_type> storage{0,};
};


template <std::integral T>
std::strong_ordering operator<=>(const large_num v, const T u){
    return v <=> static_cast<large_num>(u);
}


#define DEFINE_ARITHMETIC_OPERATION_LEFT(operation)\
template <std::integral T> \
large_num operator operation(const T v, const large_num u){ \
    return static_cast<large_num>(v) operation u; \
}

APPLY_TO_ARITHMETIC_OPERATORS(DEFINE_ARITHMETIC_OPERATION_LEFT)


#define DEFINE_ARITHMETIC_OPERATION_RIGHT(operation)\
template <std::integral T> \
large_num operator operation(const large_num v, const T u){ \
    return v operation static_cast<large_num>(u); \
}

APPLY_TO_ARITHMETIC_OPERATORS(DEFINE_ARITHMETIC_OPERATION_RIGHT)


#define DEFINE_ARITHMETIC_OPERATION_ASSIGNMENT_LEFT(operation)\
template <std::integral T> \
T &operator operation(T &v, const large_num u){ \
    return v operation static_cast<T>(u); \
}

APPLY_TO_ARITHMETIC_ASSIGNMENT_OPERATORS(DEFINE_ARITHMETIC_OPERATION_ASSIGNMENT_LEFT)


#define DEFINE_ARITHMETIC_OPERATION_ASSIGNMENT_RIGHT(operation)\
template <std::integral T> \
large_num &operator operation(large_num &v, const T u){ \
    return v operation static_cast<large_num>(u); \
}

APPLY_TO_ARITHMETIC_ASSIGNMENT_OPERATORS(DEFINE_ARITHMETIC_OPERATION_ASSIGNMENT_RIGHT)


#endif
