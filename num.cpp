#include "num.hpp"



// // large_num::large_num(const size_t bitsize, [[maybe_unused]] const initialise_by_size tag): bitsize(bitsize),
// //     v(std::vector<chunk_type>(bitsize % bits_per_chunk? bitsize / bits_per_chunk + 1 : bitsize / bits_per_chunk, 0)){}

large_num::large_num(bool value){
    large_num_storage = std::vector<chunk_type>(1);
    bitsize = 1;
    large_num_storage.at(0) = value;
}


large_num large_num::operator~() const{
    large_num res = *this;
    for (auto &part: res.large_num_storage)
        part = ~part;
    return res;
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
    return *this;
}

large_num large_num::operator-() const{
    return ++~*this;
}


large_num &large_num::operator++(){ // finish this: add extending storage
    bool needs_more_space[[maybe_unused]]; // to do: implement ---------- ---------- ---------- ----------

    for(auto &chunk : large_num_storage){
        chunk += 1;
        if (chunk == 0){ // wrap-around occured
            continue;
        }else{
            break;
        }
    }
    return *this;
}

large_num large_num::operator++(int){
    large_num old_value = *this;
    ++*this;
    return old_value;
}

large_num &large_num::operator--(){
    return *this = -++-*this;
}

large_num large_num::operator--(int){
    large_num old_value = *this;
    --*this;
    return old_value;
}


large_num large_num::operator+(const large_num &other) const{
    large_num augend = *this;
    large_num addend = other;

    if (addend.is_negative()){
        return -(-augend + -addend);
    }

    // for a positive addend: a + b = (a + 1) + (b - 1) = ... (a + b) - 0
    while (addend){ // to do: fix ---------- ---------- ---------- ----------
        ++augend;
        --addend;
    }

    return augend;
}

large_num large_num::operator-(const large_num &other) const{
    return *this + -other;
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

// std::ostream &operator<<(std::ostream &os, const large_num &n){
//     // tmp
//     return os;
// }


std::strong_ordering large_num::operator<=>(const large_num &other) const{
    // a <=> b is the same as (a - b) <=> 0
    large_num difference = *this - other;

    if (difference.is_zero()){
        return std::strong_ordering::equal;
    }else if (difference.is_negative()){
        return std::strong_ordering::less;
    }
    else{
        return std::strong_ordering::greater;
    }
}


large_num::operator bool() const{
    return !is_zero();
}


bool large_num::get_bit(const size_t n) const{
    size_t chunk_index = n / bits_per_chunk;
    size_t bit_index   = n % bits_per_chunk;
    chunk_type chunk = large_num_storage.at(chunk_index);
    size_t bit_mask = (1 << bit_index);
    bool bit = chunk & bit_mask;
    return bit;
}

void large_num::set_bit(const size_t n, bool value){
    size_t chunk_index = n / bits_per_chunk;
    size_t bit_index   = n % bits_per_chunk;
    chunk_type &chunk = large_num_storage.at(chunk_index);
    size_t bit_mask = (1 << bit_index);
    if (value){
        chunk &= bit_mask;
    }else{
        chunk |= ~bit_mask;
    }
    return;
}

bool large_num::sign_bit() const{
    size_t bit_index = bitsize - 1;
    bool bit = get_bit(bit_index);
    return bit;
}

bool large_num::is_negative() const{
    return sign_bit();
}

bool large_num::is_zero() const{
    return bitsize == 1 && !is_negative();
}

bool large_num::is_positive() const{
    return !is_negative() && !is_zero();
}

void large_num::clean_up(){
    // add: update the new bitsize ---------- ---------- ---------- ----------
    size_t sign_index = bitsize - 1;
    size_t sign_chunk_index = sign_index / bits_per_chunk;
    size_t sign_bit_index   = sign_index % bits_per_chunk;
    if (sign_bit_index == bits_per_chunk - 1){ // sign bit is in the last bit of the chunk
        large_num_storage.resize(sign_chunk_index + 1); // any chunks after that point are redundant
    }
    return;
}
