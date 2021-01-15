#include "num.hpp"

large_num::large_num(bool value) noexcept: storage(std::vector<byte_type>{value}){}

template <typename T>
constexpr size_t exponent10_max = std::numeric_limits<T>::digits10;

template <typename T>
constexpr T power10_max_calc() noexcept{
    T power10 = 1;
    for (size_t i = 0; i < exponent10_max<T>; ++i)
        power10 *= 10;
    
    return power10;
}

template <typename T>
constexpr T power10_max = power10_max_calc<T>();


constexpr size_t             digits_max = exponent10_max<unsigned long long>;
constexpr unsigned long long power_max  = power10_max   <unsigned long long>;

large_num::large_num(const std::string &number_representation){
    if (number_representation.empty())
        return;

    char first_symbol = number_representation.front();
    bool has_sign;
    bool is_negative;

    if (first_symbol == '+'){
        has_sign    = true;
        is_negative = false;
    }else if (first_symbol == '-'){
        has_sign    = true;
        is_negative = true;
    }else{
        has_sign    = false;
        is_negative = false;
    }

    if (std::any_of(number_representation.begin(), number_representation.end(), [](char c){return isspace(c);}))
        throw std::runtime_error("to be done ----------");

    size_t number_of_loose_digits_at_the_start = (number_representation.size() - has_sign) % digits_max;

    //bool has_left_over_digits = number_of_loose_digits_at_the_start;

    size_t number_of_complete_parts = (number_representation.size() - has_sign) / digits_max;

    if (number_of_loose_digits_at_the_start)
        *this = std::stoull(number_representation.substr(has_sign, number_of_loose_digits_at_the_start));

    for (size_t part_index = 0; part_index < number_of_complete_parts; ++part_index){
        *this *= power_max;
        *this += std::stoull(number_representation.substr(part_index * digits_max + number_of_loose_digits_at_the_start + has_sign, digits_max));
    }

    if (is_negative)
        negate();

    return;
}


large_num::large_num(const char *number_representation): large_num(std::string(number_representation)){}


large_num large_num::operator~() const noexcept{
    large_num result = *this;
    result.invert();
    return result;
}

large_num large_num::operator&(const large_num &other) const noexcept{
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

large_num large_num::operator|(const large_num &other) const noexcept{
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

large_num large_num::operator^(const large_num &other) const noexcept{
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


large_num large_num::operator+() const noexcept{
    return *this;
}

large_num large_num::operator-() const noexcept{
    large_num result = *this;
    result.negate();
    return result;
}

large_num &large_num::operator++() noexcept{
    return *this += 1;
}

large_num large_num::operator++(int) noexcept{
    large_num old_value = *this;
    ++*this;
    return old_value;
}

large_num &large_num::operator--() noexcept{
    return *this -= 1;
}

large_num large_num::operator--(int) noexcept{
    large_num old_value = *this;
    --*this;
    return old_value;
}

//-----
using byte_type = large_num::byte_type;
constexpr std::pair<byte_type, byte_type> byte_addition(byte_type a, byte_type b, byte_type carry = 0) noexcept(true){
    byte_type lower_byte = a + b;
    byte_type upper_byte = (lower_byte < a)? 1: 0; // wrap-over occured
    if (carry && lower_byte == static_cast<byte_type>(-1)){
        lower_byte = 0;
        upper_byte = 1;
    }else if(carry)
        lower_byte += 1;
    
    return std::pair(lower_byte, upper_byte);

}
//-----

large_num large_num::operator+(const large_num &other) const noexcept{
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

large_num large_num::operator-(const large_num &other) const noexcept{
    return *this + -other;
}


//----- to be fixed
using byte_type = large_num::byte_type;
constexpr std::pair<byte_type, byte_type> byte_multiplication(const byte_type a, const byte_type b) noexcept(true){
    // using two_byte = uintmax_t;
    // two_byte prod = (two_byte)a * (two_byte)b;

    // return std::make_pair((byte_type)(prod), (byte_type)(prod >> 8));
    constexpr size_t bits_per_byte = BITSIZEOF(byte_type);

    constexpr size_t upper_halfbyte_size = bits_per_byte / 2;
    constexpr size_t lower_halfbyte_size = bits_per_byte - upper_halfbyte_size;

    constexpr byte_type lower_halfbyte_mask = (static_cast<byte_type>(1) << lower_halfbyte_size) - 1;

    byte_type a_upper_halfbyte = a >> lower_halfbyte_size;
    byte_type a_lower_halfbyte = a &  lower_halfbyte_mask;
    byte_type b_upper_halfbyte = b >> lower_halfbyte_size;
    byte_type b_lower_halfbyte = b &  lower_halfbyte_mask;

    byte_type upper_byte = a_upper_halfbyte * b_upper_halfbyte; // carry byte
    byte_type lower_byte = a_lower_halfbyte * b_lower_halfbyte;

    byte_type between_byte1 = a_upper_halfbyte * b_lower_halfbyte;
    byte_type between_byte2 = a_lower_halfbyte * b_upper_halfbyte;

    byte_type betwee_byte1_upper_halfbyte = between_byte1 >> lower_halfbyte_size;
    byte_type betwee_byte1_lower_halfbyte = between_byte1 &  lower_halfbyte_mask;
    byte_type betwee_byte2_upper_halfbyte = between_byte2 >> lower_halfbyte_size;
    byte_type betwee_byte2_lower_halfbyte = between_byte2 &  lower_halfbyte_mask;

    byte_type carry;

    std::tie(lower_byte, carry) = byte_addition(lower_byte, betwee_byte1_lower_halfbyte << lower_halfbyte_size);
    upper_byte += carry;
    std::tie(lower_byte, carry) = byte_addition(lower_byte, betwee_byte2_lower_halfbyte << lower_halfbyte_size);
    upper_byte += carry;

    upper_byte += betwee_byte1_upper_halfbyte;
    upper_byte += betwee_byte2_upper_halfbyte;

    return std::pair(lower_byte, upper_byte);
}
//-----


large_num large_num::operator*(const large_num &other) const noexcept{
    if (is_negative())
        return -(-(*this) * other);
    if (other.is_negative())
        return -(*this * -other);
/* Karatsuba
    if ((      storage.size() == 1 || (      storage.size() == 2 &&       storage.back() == 0)) &&
        (other.storage.size() == 1 || (other.storage.size() == 2 && other.storage.back() == 0))){
        auto [lo, hi] = byte_multiplication(storage.front(), other.storage.front());
        large_num result;
        result.storage = {lo, hi};
        result.clean_up();
        return result;
    }

    const size_t common_size = std::max(storage.size(), other.storage.size());
    const size_t half_size = DIVIDE_ROUND_UP(common_size, 2);

    large_num  this_upper_half;
    large_num  this_lower_half;
    large_num other_upper_half;
    large_num other_lower_half;

    size_t  this_halfsize  = std::min(half_size, (*this).storage.size());
    size_t other_halfsize  = std::min(half_size,   other.storage.size());

    auto  this_half_storage = (*this).storage.cbegin() +  this_halfsize;
    auto other_half_storage =   other.storage.cbegin() + other_halfsize;

    this_lower_half .storage = std::vector<byte_type>((*this).storage.cbegin(), this_half_storage     );
    this_upper_half .storage = std::vector<byte_type>(this_half_storage       , (*this).storage.cend());
    other_lower_half.storage = std::vector<byte_type>(other.storage.cbegin()  , other_half_storage    );
    other_upper_half.storage = std::vector<byte_type>(other_half_storage      , other.storage.cend()  );

    this_upper_half .storage.push_back(0);
    this_lower_half .storage.push_back(0);
    other_upper_half.storage.push_back(0);
    other_lower_half.storage.push_back(0);

    this_upper_half .clean_up();
    this_lower_half .clean_up();
    other_upper_half.clean_up();
    other_lower_half.clean_up();


    // Karatsuba
    large_num result_top_part    = this_upper_half * other_upper_half;
    large_num result_bottom_part = this_lower_half * other_lower_half;
    large_num result_middle_part = (this_upper_half + this_lower_half) * (other_upper_half + other_lower_half) - 
                                   (result_top_part + result_bottom_part);

    if (result_middle_part)
    result_middle_part.storage.insert(result_middle_part.storage.begin(), half_size    , 0);
    if (result_top_part)
    result_top_part   .storage.insert(result_middle_part.storage.begin(), half_size * 2, 0);

    large_num result = result_top_part + result_middle_part + result_bottom_part;
    return result;


    // const size_t half_bitsize = half_size * bits_per_byte;
    // const size_t half_bitmaks = (static_cast<large_num>(1) << half_bitsize) - 1;

    // large_num this_upper_half  = (*this) >> half_bitsize;
    // large_num this_lower_half  = (*this) &  half_bitmaks;
    // large_num other_upper_half = other   >> half_bitsize;
    // large_num other_lower_half = other   &  half_bitmaks;
*/

    // large_num result;

    // for (size_t i = other.storage.size() * bits_per_byte; i --> 0;){
    //     result <<= 1;
    //     if (other.get_bit(i)){
    //         result += *this;
    //     }
    // }

    // return result;

    large_num result;

    for (auto byte: storage | std::views::reverse){
        result <<= bits_per_byte;
        large_num result_tmp;
        for (auto byte2: other.storage | std::views::reverse){
            result_tmp <<= bits_per_byte;
            auto [lo, hi] = byte_multiplication(byte, byte2);
            result_tmp += static_cast<large_num>(lo) + (static_cast<large_num>(hi) << bits_per_byte);
        }
        result += result_tmp;
    }

    return result;
}

large_num large_num::abs() const noexcept{
    if (is_negative())
        return -*this;
    else
        return *this;
}

std::pair<large_num, large_num> large_num::divmod(const large_num &other) const noexcept(false){
    // https://en.wikipedia.org/wiki/Division_algorithm
    if (other.is_zero())
        throw std::domain_error("You cannot divide by 0"); /* +UB+ */

    large_num N = ::abs(*this);
    large_num D = ::abs(other);
    large_num Q;
    large_num R;
    for (size_t i = N.storage.size() * bits_per_byte; i --> 0;){
        R <<= 1;
        R |=  N.get_bit(i);
        if (R >= D){
            R -= D;
            //Q.set_bit(i, 1);
            Q |= large_num(1) << i;
        }
    }

    if ((*this).is_negative()){
        Q = -Q;
        R = -R;
    }
    if (other.is_negative())
        Q = -Q;
    
    return std::pair(Q, R);
}

large_num large_num::operator/(const large_num &other) const noexcept(false){
    return ::divmod(*this, other).first;
    // if (other.is_zero()){
    //     throw std::out_of_range("You cannot divide by 0"); /* +UB+ */
    // }
    // if ((*this).is_negative()){
    //     return -(-*this / other);
    // }
    // if (other.is_negative()){
    //     return -(*this / -other);
    // }

    // if (*this < other) // idk ----------
    //     return 0;

    // large_num result;
    // large_num power = 1;
    // large_num divisor = other;

    // while (divisor <= *this){
    //     power   <<= 1;
    //     divisor <<= 1;
    // }

    // divisor >>= 1;
    // power   >>= 1;
    // result += power;

    // while (power && divisor != *this){
    //     power >>= 1;

    // }

    // return result;
}

large_num large_num::operator%(const large_num &other) const noexcept(false){
    return ::divmod(*this, other).second;
    // return *this - (*this / other) * other;
}


large_num large_num::operator<<(const large_num &other) const noexcept(false){
    if (other.is_negative())
        throw std::domain_error("You cannot shift by a negative number"); /* +UB+ */

    //size_t additional_bytes = other / static_cast<large_num>(bits_per_byte) + static_cast<large_num>(static_cast<bool>(other % static_cast<large_num>(bits_per_byte)));
    large_num result = *this;
    size_t initial_size = result.storage.size();

    size_t bytes_shifted = static_cast<size_t>(other) / bits_per_byte; // fix for numbers between __SIZE_T_MAX__ and 8 * __SIZE_T_MAX__ - 1
    size_t bits_shifted  = static_cast<size_t>(other) % bits_per_byte;

    size_t additional_bytes = bytes_shifted + static_cast<bool>(bits_shifted);

    size_t new_size = initial_size + additional_bytes;

    result.expand_upto(new_size);
    
    // to be improved ---------- ---------- ---------- ----------
    if (bytes_shifted){
        for (size_t i = initial_size; i --> 0;)
            result.storage.at(i + bytes_shifted) = result.storage.at(i);
        for (size_t i = 0; i < bytes_shifted; ++i)
            result.storage.at(i) = 0;
    }
    if (bits_shifted){
        byte_type carry;
        byte_type carry_prev = 0;
        for (size_t i = bytes_shifted; i < new_size; ++i){
            carry = result.storage.at(i) >> (bits_per_byte - bits_shifted);
            result.storage.at(i) <<= bits_shifted;
            result.storage.at(i) |= carry_prev;
            carry_prev = carry;
        }
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

large_num large_num::operator>>(const large_num &other) const noexcept(false){ // to be done ----------
    if (other.is_negative())
        throw std::domain_error("You cannot shift by a negative number"); /* +UB+ */
    if ((*this).is_negative())
        return ~(~(*this) >> other);

    // large_num result = *this;
    // large_num shift = other;
    // while(shift--){
    //     result = result / large_num(2);
    // }
    
    // return result;

    large_num result = *this;
    size_t initial_size = result.storage.size();

    size_t bytes_shifted = static_cast<size_t>(other) / bits_per_byte; // fix for numbers between __SIZE_T_MAX__ and 8 * __SIZE_T_MAX__ - 1
    size_t bits_shifted  = static_cast<size_t>(other) % bits_per_byte;

    // size_t additional_bytes = bytes_shifted + static_cast<bool>(bits_shifted);

    size_t new_size = initial_size - bytes_shifted;

    // result.expand_upto(new_size);
    
    // to be improved ---------- ---------- ---------- ----------
    if (bytes_shifted){
        for (size_t i = 0; i < new_size; ++i)
            result.storage.at(i) = result.storage.at(i + bytes_shifted);
        for (size_t i = new_size; i < initial_size; ++i)
            result.storage.at(i) = 0;
    }
    if (bits_shifted){
        byte_type shifted_carry;
        byte_type shifted_carry_prev = 0;
        for (size_t i = new_size; i --> 0;){
            shifted_carry = result.storage.at(i) << (bits_per_byte - bits_shifted);
            result.storage.at(i) >>= bits_shifted;
            result.storage.at(i) |= shifted_carry_prev;
            shifted_carry_prev = shifted_carry;
            // carry = result.storage.at(i) >> (bits_per_byte - bits_shifted);
            // result.storage.at(i) <<= bits_shifted;
            // result.storage.at(i) |= carry_prev;
            // carry_prev = carry;
        }
    }

    result.clean_up();
    return result;
}

#define DEFINE_ASSIGNMENT(operation)\
large_num &large_num::operator operation##=(const large_num &other) noexcept(noexcept(std::declval<large_num>() operation std::declval<large_num>())){ \
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


std::strong_ordering large_num::operator<=>(const large_num &other) const noexcept{
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

    if (difference.is_positive())
        return std::strong_ordering::greater;
    else
        return std::strong_ordering::less;
}


large_num::operator bool() const noexcept{
    return !is_zero();
}


large_num::operator std::string() const{ // reimplement using to_string on unsigned long long chunks ----------
    large_num num = ::abs(*this);
    bool is_negative = (*this).is_negative();

    std::string number_representation;

    while (num > power_max){
        unsigned long long small_piece;
        std::tie(num, small_piece) = ::divmod(num, power_max);
        std::string small_piece_representaion = std::to_string(small_piece);
        small_piece_representaion = std::string(digits_max - small_piece_representaion.size(), '0') + small_piece_representaion;
        number_representation = small_piece_representaion + number_representation;
    }
    unsigned long long small_piece = num;
    std::string small_piece_representaion = std::to_string(small_piece);
    number_representation = small_piece_representaion + number_representation;

    if (is_negative)
        number_representation = '-' + number_representation;

    return number_representation;


    // std::string s;
    // large_num num = *this;

    // bool is_negative;

    // if (num.is_negative()){
    //     is_negative = true;
    //     // s.append('-');
    //     num = -num;
    // }else{
    //     is_negative = false;
    // }

    // std::string reversed_digits;
    // do {
    //     auto [upper_digits, lowest_digit] = ::divmod(num, 10);
    //     char digit = '0' + static_cast<char>(lowest_digit);
    //     reversed_digits.push_back(digit);
    //     num = std::move(upper_digits);
    // }while (num);

    // std::reverse(reversed_digits.begin(), reversed_digits.end());
    // if (is_negative){
    //     reversed_digits.insert(0, 1, '-');
    // }

    // return reversed_digits;
}


std::ostream &operator<<(std::ostream &os, const large_num &n){
    return os << std::string(n);
    // large_num num = n;

    // if (num.is_negative()){
    //     os << '-';
    //     num = -num;
    // }

    // std::vector<char> reversed_digits;
    // do {
    //     auto [upper_digits, lowest_digit] = num.divmod(10);
    //     char digit = '0' + static_cast<char>(lowest_digit);
    //     reversed_digits.push_back(digit);
    //     num = std::move(upper_digits);
    // }while (num);

    // for (auto digit: std::ranges::reverse_view(reversed_digits)){
    //     os << digit;
    // }

    // return os;
}

std::istream &operator>>(std::istream &is, large_num &n){
    std::string number_representation;
    is >> number_representation;
    n = large_num(number_representation);
    return is;
}


void large_num::invert() noexcept{
    for (auto &part: storage)
        part = ~part;
    return;
}


void large_num::negate() noexcept{
    invert();
    ++*this;
}




void large_num::expand_upto(size_t n) noexcept(false){
    const size_t old_size = storage.size();
    const size_t new_size = n;

    if (new_size <= old_size)
        return;

    storage.resize(new_size, is_negative()? -1: 0);
}


bool large_num::get_bit(size_t n) const noexcept{
    size_t max_bit_index = storage.size() * bits_per_byte - 1;
    n = std::min(n, max_bit_index); // bits past-the-end are the same as the msb

    size_t byte_index = n / bits_per_byte;
    size_t bit_index  = n % bits_per_byte;

    byte_type byte = storage.at(byte_index);
    byte_type bit_mask = (static_cast<byte_type>(1) << bit_index);
    bool bit = byte & bit_mask;
    return bit;
}

// void large_num::set_bit(const size_t n, bool value){ // add: extention on past-the-end bit setting ---------- ---------- ----------
//     if (get_bit(n) == value){
//         return;
//     }
//     size_t byte_index = n / bits_per_byte;
//     size_t bit_index  = n % bits_per_byte;

//     expand_upto(byte_index + 1);

//     byte_type &byte = storage.at(byte_index);
//     size_t bit_mask = (1 << bit_index);

//     if (value){
//         byte &= bit_mask;
//     }else{
//         byte |= ~bit_mask;
//     }
    
//     clean_up();
// }

bool large_num::sign_bit() const noexcept{
    size_t bit_index = storage.size() * bits_per_byte - 1;
    bool bit = get_bit(bit_index);
    return bit;
}

bool large_num::is_negative() const noexcept{
    return sign_bit();
}

bool large_num::is_zero() const noexcept{
    return storage.size() == 1 && storage.at(0) == static_cast<byte_type>(0);
}

bool large_num::is_positive() const noexcept{
    return !is_negative() && !is_zero();
}

void large_num::clean_up() noexcept{
    byte_type sign_byte = storage.back();
    bool sign_bit_value;
    if (sign_byte == static_cast<byte_type>(-1))
        sign_bit_value = true;
    else if (sign_byte == static_cast<byte_type>(0))
        sign_bit_value = false;
    else
        return;
    
    size_t new_size = storage.size();
    byte_type current_byte;
    byte_type lower_byte = storage.back();
    while (new_size > 1){
        current_byte = lower_byte;
        lower_byte   = storage.at(new_size - 2);
        //byte_type byte = storage.at(new_size - 1);
        //bool lower_byte_msb = get_bit((new_size - 1) * bits_per_byte - 1);
        if (current_byte == sign_byte && (lower_byte >> (bits_per_byte - 1)) == sign_bit_value)
            --new_size;
        else
            break;
    }

    storage.resize(new_size);
}

large_num abs(const large_num& n) noexcept{
    return n.abs();
}

std::pair<large_num, large_num> divmod(const large_num &a, const large_num &b) noexcept(false){
    return a.divmod(b);
}
