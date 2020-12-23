#ifndef NUM_HPP
#define NUM_HPP

#include <vector>
#include <iostream>

#define BITSIZEOF(T) sizeof(T) * __CHAR_BIT__

// struct large_num;

// std::ostream &operator<<(std::ostream &os, const large_num &n);

struct large_num{
    using base = uint_fast8_t;
    static_assert(std::is_unsigned_v<base>);
    static constexpr size_t base_bit_size = BITSIZEOF(base);

    public:
        explicit large_num() = default;
        // large_num(const large_num &other) = default;
        // large_num(large_num &&other) = default;
        template<std::integral T>
        large_num(const T v): large_num(){}

        // large_num &operator=(const large_num &other) = default;
        // large_num &operator=(large_num &&other) = default;
        // template<std::integral T>
        // large_num &operator=(const T v);

        large_num operator~() const;
        // large_num operator&(const large_num &other) const;
        // large_num operator|(const large_num &other) const;
        // large_num operator^(const large_num &other) const;

        large_num operator+() const;
        large_num operator-() const;

        large_num &operator++();
        large_num operator++(int);
        large_num &operator--();
        large_num operator--(int);

        large_num operator+(const large_num &other) const;
        large_num operator-(const large_num &other) const;
        // large_num operator*(const large_num &other) const;
        // large_num operator/(const large_num &other) const;
        // large_num operator%(const large_num &other) const;

        // large_num operator<<(const large_num &other) const;
        // large_num operator>>(const large_num &other) const;

        std::strong_ordering operator<=>(const large_num &other) const;

        bool operator==(const large_num &other) const = default;
        // bool operator!=(const large_num &other) const; // candidate for deletion

        explicit operator bool() const;

        template<std::integral T>
        operator T() const{
            // tmp
            return 0;
        }

        // friend std::ostream &operator<<(std::ostream &os, const large_num &n);
    // private:
        // //enum class initialise_by_size;
        // //static initialise_by_size by_size;
        // //large_num(const size_t bitsize, const initialise_by_size tag);
        // bool get_bit(const size_t n) const;
        // bool set_bit(const size_t n);
        // bool sign_bit() const;
        // void compactify();
    private:
        std::vector<base> v;
        size_t bitsize = 0;
};

#endif
