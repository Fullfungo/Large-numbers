#include "num.hpp"

large_num::large_num(bool value): storage(std::vector<byte_type>{value}){}


large_num large_num::operator~() const{
    large_num res = *this;
    for (auto &part: res.storage)
        part = ~part;
    return res;
}

large_num large_num::operator&(const large_num &other) const{
    large_num argument1 = *this;
    large_num argument2 = other;

    size_t common_size = std::max(argument1.storage.size(), argument2.storage.size());

    argument1.expand_upto(common_size);
    argument2.expand_upto(common_size);

    for (size_t i = 0; i < common_size; ++i)
        argument1.storage.at(i) &= argument2.storage.at(i);
    
    argument1.clean_up();
    return argument1;
}

large_num large_num::operator|(const large_num &other) const{
    large_num argument1 = *this;
    large_num argument2 = other;

    size_t common_size = std::max(argument1.storage.size(), argument2.storage.size());

    argument1.expand_upto(common_size);
    argument2.expand_upto(common_size);

    for (size_t i = 0; i < common_size; ++i)
        argument1.storage.at(i) |= argument2.storage.at(i);
    
    argument1.clean_up();
    return argument1;
}

large_num large_num::operator^(const large_num &other) const{
    large_num argument1 = *this;
    large_num argument2 = other;

    size_t common_size = std::max(argument1.storage.size(), argument2.storage.size());

    argument1.expand_upto(common_size);
    argument2.expand_upto(common_size);

    for (size_t i = 0; i < common_size; ++i)
        argument1.storage.at(i) ^= argument2.storage.at(i);
    
    argument1.clean_up();
    return argument1;
}


large_num large_num::operator+() const{
    return *this;
}

large_num large_num::operator-() const{
    return ++~*this;
}

large_num &large_num::operator++(){ // finish this: add extending storage
    return *this += 1;
}

large_num large_num::operator++(int){
    large_num old_value = *this;
    ++*this;
    return old_value;
}

large_num &large_num::operator--(){
    return *this -= 1;
}

large_num large_num::operator--(int){
    large_num old_value = *this;
    --*this;
    return old_value;
}

//-----
using byte_type = large_num::byte_type;
constexpr std::pair<byte_type, byte_type> byte_addition(byte_type a, byte_type b, byte_type carry) noexcept(true){
    byte_type lower_byte = a + b;
    byte_type upper_byte = (lower_byte < a)? 1: 0; // wrap-over occured
    if (carry && lower_byte == static_cast<byte_type>(-1)){
        lower_byte = 0;
        upper_byte = 1;
    }else if(carry){
        lower_byte += 1;
    }
    return std::pair(lower_byte, upper_byte);

}
//-----

large_num large_num::operator+(const large_num &other) const{
    large_num augend = *this;
    large_num addend = other;

    const size_t common_size = std::max(augend.storage.size(), addend.storage.size());

    augend.expand_upto(common_size + 1);
    addend.expand_upto(common_size + 1);

    byte_type carry = 0;

    for (size_t i = 0; i <= common_size; ++i){
        auto [lower_byte, upper_byte] = byte_addition(augend.storage.at(i), addend.storage.at(i), carry);
        augend.storage.at(i) = lower_byte;
        carry = upper_byte;
    }

    augend.clean_up();
    return augend;
}

large_num large_num::operator-(const large_num &other) const{
    return *this + -other;
}

large_num large_num::operator*(const large_num &other) const{
    if (other.is_negative()){
        return -(*this * -other);
    }

    const large_num &multiplier = *this;
    large_num multiplicand = other;
    large_num product;

    while (multiplicand--){
        product = product + multiplier;
    }
    
    return product;
}

large_num large_num::operator/(const large_num &other) const{
    if (other.is_zero()){
        throw std::out_of_range("You cannot divide by 0"); /* +UB+ */
    }
    if ((*this).is_negative()){
        return -(-*this / other);
    }
    if (other.is_negative()){
        return -(*this / -other);
    }

    large_num dividend = *this;
    const large_num &divisor = other;
    large_num quotient;

    while (!((dividend = dividend - divisor).is_negative())){
        ++quotient;
    }
    
    return quotient;
}

large_num large_num::operator%(const large_num &other) const{
    return *this - (*this / other) * other;
}


large_num large_num::operator<<(const large_num &other) const{
    if (other.is_negative()){
        throw std::out_of_range("You cannot shift by a negative number"); /* +UB+ */
    }

    size_t additional_bytes = other / static_cast<large_num>(bits_per_byte) + static_cast<large_num>(static_cast<bool>(other % static_cast<large_num>(bits_per_byte)));
    large_num result = *this;
    size_t initial_size = result.storage.size();
    result.expand_upto(result.storage.size() + additional_bytes);
    size_t bytes_shifted = other / bits_per_byte;
    [[maybe_unused]]size_t bits_shifted  = other % bits_per_byte;
    
    // to be improved ---------- ---------- ---------- ----------
    for (size_t i = initial_size; i --> 0;){
        result.storage.at(i + bytes_shifted) = result.storage.at(i);
    }
    for (size_t i = 0; i < bytes_shifted; ++i){
        result.storage.at(i) = 0;
    }
    byte_type carry;
    byte_type carry_prev = 0;
    for (size_t i = bytes_shifted; i < result.storage.size(); ++i){
        carry = result.storage.at(i) >> (bits_per_byte - bits_shifted);
        result.storage.at(i) <<= bits_shifted;
        result.storage.at(i) |= carry_prev;
        carry_prev = carry;
    }
    
    
//tmp
    // large_num result = *this;
    // large_num shift = other;
    // while(shift--){
    //     result = result * large_num(2);
    // }
    result.clean_up();
    return result;
}

large_num large_num::operator>>(const large_num &other) const{
    if (other.is_negative()){
        throw std::out_of_range("You cannot shift by a negative number"); /* +UB+ */
    }
    if ((*this).is_negative()){
        return ~(~*this >> other);
    }

    large_num result = *this;
    large_num shift = other;
    while(shift--){
        result = result / large_num(2);
    }
    
    return result;
}

#define DEFINE_ASSIGNMENT(operation)\
large_num &large_num::operator operation##=(const large_num &other){ \
    return *this = *this operation other; \
}

#define AP(...) APPLY_TO_ARITHMETIC_OPERATORS(__VA_ARGS__)

AP(DEFINE_ASSIGNMENT)

// large_num &large_num::operator&=(const large_num &other){
//     return *this = *this & other;
// }

// large_num &large_num::operator|=(const large_num &other){
//     return *this = *this | other;
// }

// large_num &large_num::operator^=(const large_num &other){
//     return *this = *this ^ other;
// }

// large_num &large_num::operator+=(const large_num &other){
//     return *this = *this + other;
// }

// large_num &large_num::operator-=(const large_num &other){
//     return *this = *this - other;
// }

// large_num &large_num::operator*=(const large_num &other){
//     return *this = *this * other;
// }

// large_num &large_num::operator/=(const large_num &other){
//     return *this = *this / other;
// }

// large_num &large_num::operator%=(const large_num &other){
//     return *this = *this % other;
// }

// large_num &large_num::operator<<=(const large_num &other){
//     return *this = *this << other;
// }

// large_num &large_num::operator>>=(const large_num &other){
//     return *this = *this >> other;
// }


// std::ostream &operator<<(std::ostream &os, const large_num &n){
//     // tmp
//     return os;
// }


std::strong_ordering large_num::operator<=>(const large_num &other) const{
    // fast checks
    //     different signs
    const auto sign1 =       is_positive() -       is_negative();
    const auto sign2 = other.is_positive() - other.is_negative();
    if (sign1 > sign2)
        return std::strong_ordering::greater;
    if (sign1 < sign2)
        return std::strong_ordering::less;
    if (sign1 == 0 && sign2 == 0)
        return std::strong_ordering::equal;

    //     same sign, different size
    const auto &storage1 =       storage;
    const auto &storage2 = other.storage;
    const size_t size1 = storage1.size();
    const size_t size2 = storage2.size();
    if (sign1 == +1 && sign2 == +1 && size1 != size2)
        return size1 <=> size2;
    if (sign1 == -1 && sign2 == -1 && size1 != size2)
        return size2 <=> size1;

    //     same sign, same size
    if (sign1 == sign2 && storage1 == storage2)
        return std::strong_ordering::equal;
    
    // slow general check
    // a <=> b is the same as (a - b) <=> 0
    large_num difference = *this - other;

    if (difference.is_positive()){
        return std::strong_ordering::greater;
    }else{
        return std::strong_ordering::less;
    }
}


large_num::operator bool() const{
    return !is_zero();
}


std::ostream &operator<<(std::ostream &os, const large_num &n){
    large_num num = n;
    if (n < 0){
        os << '-';
        num = -n;
    }
    std::vector<char> reversed_digits;
    while (num){
        char digit = '0' + num % 10;
        reversed_digits.push_back(digit);
        num /= 10;
    }
    for (auto digit: std::ranges::reverse_view(reversed_digits)){
        os << digit;
    }
    return os;
    // // tmp
    // os << "Just a long number at " << &n << ".\n";
    // return os;
}

std::istream &operator>>(std::istream &is, const large_num &n){
    // tmp
    return is;
}


void large_num::expand_upto(size_t n){
    const size_t old_size = storage.size();
    const size_t new_size = n;

    if (new_size <= old_size){
        return;
    }

    storage.resize(new_size, is_negative()? -1: 0);
}


bool large_num::get_bit(size_t n) const{
    size_t max_bit_index = storage.size() * bits_per_byte - 1;
    n = std::min(n, max_bit_index); // bits past-the-end are the same as the msb

    size_t byte_index = n / bits_per_byte;
    size_t bit_index  = n % bits_per_byte;

    byte_type byte = storage.at(byte_index);
    size_t bit_mask = (1 << bit_index);
    bool bit = byte & bit_mask;
    return bit;
}

void large_num::set_bit(const size_t n, bool value){ // add: extention on past-the-end bit setting ---------- ---------- ----------
    if (get_bit(n) == value){
        return;
    }
    size_t byte_index = n / bits_per_byte;
    size_t bit_index  = n % bits_per_byte;

    byte_type &byte = storage.at(byte_index);
    size_t bit_mask = (1 << bit_index);

    if (value){
        byte &= bit_mask;
    }else{
        byte |= ~bit_mask;
    }
    
    clean_up();
}

bool large_num::sign_bit() const{
    size_t bit_index = storage.size() * bits_per_byte - 1;
    bool bit = get_bit(bit_index);
    return bit;
}

bool large_num::is_negative() const{
    return sign_bit();
}

bool large_num::is_zero() const{
    return storage.size() == 1 && storage.at(0) == static_cast<byte_type>(0);
}

bool large_num::is_positive() const{
    return !is_negative() && !is_zero();
}

void large_num::clean_up(){
    size_t new_size = storage.size();
    while (new_size > 1){
        byte_type byte = storage.at(new_size - 1);
        bool lower_byte_msb = get_bit((new_size - 1) * bits_per_byte - 1);
        if ((byte == static_cast<byte_type>( 0) && lower_byte_msb == 0)||
            (byte == static_cast<byte_type>(-1) && lower_byte_msb == 1)){
            --new_size;
        }else{
            break;
        }
    }

    storage.resize(new_size);
}
