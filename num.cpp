#include "num.hpp"



// // large_num::large_num(const size_t bitsize, [[maybe_unused]] const initialise_by_size tag): bitsize(bitsize),
// //     v(std::vector<base>(bitsize % base_bit_size? bitsize / base_bit_size + 1 : bitsize / base_bit_size, 0)){}





large_num large_num::operator~() const{
    // large_num res = *this;
    // for (auto &part: res.v)
    //     part = ~part;
    // return res;
    return *this;
}

// large_num large_num::operator&(const large_num &other) const{
//     // tmp
//     large_num res(std::min(bitsize, other.bitsize), by_size);
//     for (size_t i = 0; i < res.v.size(); ++i)
//         res.v[i] = v[i] & other.v[i];
//     return *this;
// }

// large_num large_num::operator|(const large_num &other) const{
//     // extend sign_bits
//     const large_num &longer  = (v.size() >= other.v.size())? *this: other;
//     const large_num &shorter = (v.size() >= other.v.size())? other: *this;
//     large_num res(longer.bitsize, by_size);
//     for (size_t i = 0; i < shorter.v.size(); ++i)
//             res.v[i] = longer.v[i] | shorter.v[i];
//     for (size_t i = shorter.v.size(); i < longer.v.size(); ++i)
//         if (shorter.sign_bit())
//             res.v[i] = -1;
//         else
//             res.v[i] = longer.v[i];
//     return res;
// }

// large_num large_num::operator^(const large_num &other) const{
//     // tmp
//     return *this;
// }


large_num large_num::operator+() const{
    // tmp
    return *this;
}

large_num large_num::operator-() const{
    // tmp
    return *this;
}


large_num &large_num::operator++(){
    // tmp
    return *this;
}

large_num large_num::operator++(int){
    // tmp
    return *this;
}

large_num &large_num::operator--(){
    // tmp
    return *this;
}

large_num large_num::operator--(int){
    // tmp
    return *this;
}


large_num large_num::operator+(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator-(const large_num &other) const{
    // tmp
    return *this;
}

// large_num large_num::operator*(const large_num &other) const{
//     // tmp
//     return *this;
// }

// large_num large_num::operator/(const large_num &other) const{
//     // tmp
//     return *this;
// }

// large_num large_num::operator%(const large_num &other) const{
//     // tmp
//     return *this;
// }


// large_num large_num::operator<<(const large_num &other) const{
//     // tmp
//     return *this;
// }

// large_num large_num::operator>>(const large_num &other) const{
//     // tmp
//     return *this;
// }


std::strong_ordering large_num::operator<=>(const large_num &other) const{
    // tmp
    return std::strong_ordering::equal;
}


large_num::operator bool() const{
    // tmp
    return false;
}


// std::ostream &operator<<(std::ostream &os, const large_num &n){
//     // tmp
//     return os;
// }

// bool large_num::get_bit(const size_t n) const{
//     // tmp
//     return false;
// }

// bool large_num::set_bit(const size_t n){
//     // tmp
//     return false;
// }

// bool large_num::sign_bit() const{
//     // tmp
//     return false;
// }

// void large_num::compactify(){
//     // tmp
//     return;
// }
