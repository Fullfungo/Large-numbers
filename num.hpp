#ifndef NUM_HPP
#define NUM_HPP

#include <vector>
#include <iostream>
#include <iostream>

#define BITSIZEOF(T) sizeof(T) * __CHAR_BIT__

// struct large_num;

// std::ostream &operator<<(std::ostream &os, const large_num &n);

#define PRIVATE_ACCESS 1

#if PRIVATE_ACCESS
    #define private public
#endif

// helper function
constexpr size_t divide_round_up(size_t a, size_t b) noexcept{
    return a / b + static_cast<bool>(a % b);
}

struct large_num{
    using chunk_type = uint_fast8_t;
    static_assert(std::is_unsigned_v<chunk_type>);
    static constexpr size_t bits_per_chunk = BITSIZEOF(chunk_type);

    public:
        explicit large_num() = default;
        // large_num(const large_num &other) = default;
        // large_num(large_num &&other) = default;
        template<std::integral T>
        large_num(T value){
                static_assert(!std::is_same_v<T, bool>);
            constexpr size_t storage_size = divide_round_up(sizeof(T), sizeof(chunk_type));
            large_num_storage = std::vector<chunk_type>(storage_size, 0);
            bitsize = BITSIZEOF(T);
            // add clean-up ---------- ---------- ---------- ----------
            constexpr chunk_type chunk_mask = ~static_cast<chunk_type>(0);
            for (auto &chunk : large_num_storage){
                chunk = value & chunk_mask;
                value >>= bits_per_chunk;
            }
        }

        large_num(bool value);

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

        operator bool() const;

        template<std::integral T>
        operator T() const{
                static_assert(!std::is_same_v<T, bool>);
            T value = 0;
            constexpr size_t chunks_to_read = divide_round_up(sizeof(T), sizeof(chunk_type));
            for (size_t i = chunks_to_read; i --> 0;){
                value <<= bits_per_chunk;
                value |= large_num_storage.at(i);
            }
            return value;
        }

        // friend std::ostream &operator<<(std::ostream &os, const large_num &n);
    private:
        // //enum class initialise_by_size;
        // //static initialise_by_size by_size;
        // //large_num(const size_t bitsize, const initialise_by_size tag);
        bool get_bit(const size_t n) const;
        void set_bit(const size_t n, bool value);
        bool sign_bit() const;
        bool is_negative() const;
        bool is_zero() const;
        bool is_positive() const;
        void clean_up();
    private:
        std::vector<chunk_type> large_num_storage = std::vector<chunk_type>(1, 0);
        size_t bitsize = 1; // to do: remove ---------- ---------- ---------- ----------
};

#endif
