#ifndef FRACTION_HPP
#define FRACTION_HPP

#include "num.hpp"

class large_fraction;

std::ostream &operator<<(std::ostream &os, const large_fraction &f);
std::istream &operator>>(std::istream &is, large_fraction &f);

class large_fraction{
    public:
        large_fraction() = default;
        large_fraction(const large_fraction &) = default;
        large_fraction(large_fraction &&) = default;
        large_fraction &operator=(const large_fraction &) = default;
        large_fraction &operator=(large_fraction &&) = default;

        large_fraction(const large_num &a, const large_num &b);
        large_fraction(const large_num &a);

        large_fraction operator+() const;
        large_fraction operator-() const;

        large_fraction operator+(const large_fraction &other) const;
        large_fraction operator-(const large_fraction &other) const;
        large_fraction operator*(const large_fraction &other) const;
        large_fraction operator/(const large_fraction &other) const;

        std::strong_ordering operator<=>(const large_fraction &other) const;
        bool operator==(const large_fraction &) const = default;

        explicit operator bool() const;
        explicit operator large_num() const;
    private:
        void reduce();
    private:
        large_num numerator   = 0;
        large_num denominator = 1;
};

#endif
