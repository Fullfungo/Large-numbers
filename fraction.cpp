#include "fraction.hpp"

large_fraction::large_fraction(const large_num &a, const large_num &b): numerator(a), denominator(b){
    if (!b)
        throw std::out_of_range("denominator cannot equal 0");
    reduce();
}

large_fraction::large_fraction(const large_num &a): large_fraction(a, 1){}

large_fraction large_fraction::operator+() const{
    return *this;
}

large_fraction large_fraction::operator-() const{
    large_fraction result = *this;
    result.numerator = -result.numerator;
    return result;
}

large_fraction large_fraction::operator+(const large_fraction &other) const{
    large_fraction result;
    result.numerator   = numerator * other.denominator + denominator * other.numerator;
    result.denominator = denominator * other.denominator;
    result.reduce();
    return result;
}

large_fraction large_fraction::operator-(const large_fraction &other) const{
    return (*this) + -other;
}

large_fraction large_fraction::operator*(const large_fraction &other) const{
    large_fraction result;
    result.numerator   = numerator * other.numerator;
    result.denominator = denominator * other.denominator;
    result.reduce();
    return result;
}

large_fraction large_fraction::operator/(const large_fraction &other) const{
    large_fraction result;
    result.numerator   = numerator * other.denominator;
    result.denominator = denominator * other.numerator;
    result.reduce();
    return result;
}

std::strong_ordering large_fraction::operator<=>(const large_fraction &other) const{
    return (numerator * other.denominator - denominator * other.numerator) <=> 0;
}

large_fraction::operator bool() const{
    return numerator;
}

large_fraction::operator large_num() const{
    return numerator / denominator;
}

large_num gcd(const large_num &a, const large_num &b){
    large_num num1 = abs(a);
    large_num num2 = abs(b);
    while (num2){
        std::tie(num1, num2) = std::make_pair(num2, num1 % num2);
    }
    return num1;
}

void large_fraction::reduce(){
    large_num common_factor = gcd(numerator, denominator);
    if (common_factor != 1){
        numerator   /= common_factor;
        denominator /= common_factor;
    }
    if (denominator < 0){
        numerator   = -numerator;
        denominator = -denominator;
    }
    return;
}

std::ostream &operator<<(std::ostream &os, const large_fraction &f){
    os << f.numerator;
    if (f.denominator > 1)
        os << '/' << f.denominator;
    return os;
}

std::istream &operator>>(std::istream &is, large_fraction &f){
    std::string s;
    is >> s;
    size_t delimiter_position = s.find('/');
    if (delimiter_position == std::string::npos){
        f.numerator   = static_cast<large_num>(s);
        f.denominator = static_cast<large_num>(1);
    }else{
        f.numerator   = static_cast<large_num>(s.substr(0, delimiter_position));
        f.denominator = static_cast<large_num>(s.substr(delimiter_position + 1));
    }
    return is;
}
