#include "num.hpp"



// // large_num::large_num(const size_t bitsize, [[maybe_unused]] const initialise_by_size tag): bitsize(bitsize),
// //     v(std::vector<chunk_type>(bitsize % bits_per_chunk? bitsize / bits_per_chunk + 1 : bitsize / bits_per_chunk, 0)){}

large_num::large_num(bool value){
    storage = std::vector<chunk_type>{value};
    return;
}


large_num large_num::operator~() const{
    large_num res = *this;
    for (auto &part: res.storage)
        part = ~part;
    return res;
}

large_num large_num::operator&(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator|(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator^(const large_num &other) const{
    // tmp
    return *this;
}


large_num large_num::operator+() const{
    return *this;
}

large_num large_num::operator-() const{
    return ++~*this;
}


large_num &large_num::operator++(){ // finish this: add extending storage
    bool needs_more_space[[maybe_unused]]; // to do: implement ---------- ---------- ---------- ----------

    for(auto &chunk : storage){
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

large_num large_num::operator*(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator/(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator%(const large_num &other) const{
    // tmp
    return *this;
}


large_num large_num::operator<<(const large_num &other) const{
    // tmp
    return *this;
}

large_num large_num::operator>>(const large_num &other) const{
    // tmp
    return *this;
}

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


bool large_num::get_bit(size_t n) const{
    size_t max_bit_index = storage.size() * bits_per_chunk - 1;
    n = std::min(n, max_bit_index); // bits past-the-end are the same as the msb

    size_t chunk_index = n / bits_per_chunk;
    size_t bit_index   = n % bits_per_chunk;

    chunk_type chunk = storage.at(chunk_index);
    size_t bit_mask = (1 << bit_index);
    bool bit = chunk & bit_mask;
    return bit;
}

void large_num::set_bit(const size_t n, bool value){ // add: extention on past-the-end bit setting ---------- ---------- ----------
    if (get_bit(n) == value){
        return;
    }
    size_t chunk_index = n / bits_per_chunk;
    size_t bit_index   = n % bits_per_chunk;

    chunk_type &chunk = storage.at(chunk_index);
    size_t bit_mask = (1 << bit_index);

    if (value){
        chunk &= bit_mask;
    }else{
        chunk |= ~bit_mask;
    }
    
    clean_up();

    return;
}

bool large_num::sign_bit() const{
    size_t bit_index = storage.size() * bits_per_chunk - 1;
    bool bit = get_bit(bit_index);
    return bit;
}

bool large_num::is_negative() const{
    return sign_bit();
}

bool large_num::is_zero() const{
    return storage.size() == 1 && storage.at(0) == static_cast<chunk_type>(0);
}

bool large_num::is_positive() const{
    return !is_negative() && !is_zero();
}

void large_num::clean_up(){
    size_t new_size = storage.size();
    while (new_size > 1){
        chunk_type chunk = storage.at(new_size - 1);
        bool lower_chunk_msb = get_bit((new_size - 1) * bits_per_chunk - 1);
        if ((chunk == static_cast<chunk_type>( 0) && lower_chunk_msb == 0) ||
            (chunk == static_cast<chunk_type>(~0) && lower_chunk_msb == 1)){
            --new_size;
        }else{
            break;
        }
    }

    storage.resize(new_size);

    return;
}
