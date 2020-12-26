#ifndef NUM_HPP
#define NUM_HPP

#include <vector>
#include <iostream>
#include <iostream>
#include <concepts>
#include <ranges>

#define BITSIZEOF(T) sizeof(T) * __CHAR_BIT__

// struct large_num;

// std::ostream &operator<<(std::ostream &os, const large_num &n);

#define PRIVATE_ACCESS 0

#if PRIVATE_ACCESS
    #define private public
#endif

// helper function
constexpr size_t divide_round_up(size_t a, size_t b) noexcept{
    return a / b + static_cast<bool>(a % b);
}

struct large_num{
    using chunk_type = uint_least8_t; // want: change to uint_fast8_t
    static_assert(std::is_unsigned_v<chunk_type>);
    static constexpr size_t bits_per_chunk = BITSIZEOF(chunk_type);

    public:
        explicit large_num() = default;
        // large_num(const large_num &other) = default;
        // large_num(large_num &&other) = default;
        template<std::integral T>
        large_num(T value){
                // static_assert(!std::is_same_v<T, bool>);
            constexpr bool may_need_sign_bit = std::is_unsigned_v<T>;
            constexpr size_t storage_size = divide_round_up(BITSIZEOF(T) + may_need_sign_bit, BITSIZEOF(chunk_type));
            storage = std::vector<chunk_type>(storage_size, 0);
            // bitsize = BITSIZEOF(T);
            constexpr chunk_type chunk_mask = static_cast<chunk_type>(~0);
            for (auto &chunk : storage){
                // assumes every standard integral type has a size of power of 2 bytes and chunk_type is the smallest (bad)
                chunk = value & chunk_mask;
                value >>= bits_per_chunk;
            }

            clean_up();

            return;
        }

        large_num(bool value);

        // large_num &operator=(const large_num &other) = default;
        // large_num &operator=(large_num &&other) = default;
        // template<std::integral T>
        // large_num &operator=(const T v);

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

        large_num operator&=(const large_num &other);
        large_num operator|=(const large_num &other);
        large_num operator^=(const large_num &other);

        large_num operator+=(const large_num &other);
        large_num operator-=(const large_num &other);
        large_num operator*=(const large_num &other);
        large_num operator/=(const large_num &other);
        large_num operator%=(const large_num &other);

        large_num operator<<=(const large_num &other);
        large_num operator>>=(const large_num &other);

        std::strong_ordering operator<=>(const large_num &other) const;

        bool operator==(const large_num &other) const = default;
        // bool operator!=(const large_num &other) const; // candidate for deletion

        operator bool() const;

        template<std::integral T>
        operator T() const{
                // static_assert(!std::is_same_v<T, bool>);
            T value = is_negative() ? ~static_cast<T>(0) : static_cast<T>(0);
            constexpr size_t chunks_to_read = divide_round_up(sizeof(T), sizeof(chunk_type));
            for (auto chunk : storage | std::views::take(chunks_to_read) | std::views::reverse){
                value <<= bits_per_chunk;
                value |= chunk;
            }
            return value;
        }

        // friend std::ostream &operator<<(std::ostream &os, const large_num &n);
    private:
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
        std::vector<chunk_type> storage{0};
};

#endif
