#pragma once

#include "InfInt.h"
#include <deque>
#include <string>
#include <iostream> // just for exceptions


/* constructors */

InfInt::InfInt()
{
    value.push_back(0);
};

InfInt::InfInt(const std::string& s)
{
    if(s.empty())
        value.push_back(0);
    else for(int i = 0; i < s.size(); ++i){
        value.push_back(s[i] == '1');
    }
    truncate_front_zeros();
};

InfInt::InfInt(int i)
{
    bool remainder = false; //converting to binary
    do{
        remainder = i % 2;
        i /= 2;
        value.push_front(remainder);
    } while(i != 0);
};

InfInt::InfInt(const InfInt& ii)
{
    this->value = ii.value;
};

/* private functions */

void InfInt::resize_front(const size_t& new_size, bool filler)
{
    value.insert(value.begin(), new_size - size(), filler);
}

InfInt InfInt::truncate_front_zeros()
{
    if(value[0])
        return *this;
    size_t initial_size = size();
    for(int i = 0; i < initial_size; ++i){
        if(!value.front())
            value.pop_front();
        else break;
    }
    if(value.empty())
        value.push_back(0);
    return *this;
}

/* operators */

InfInt& InfInt::operator=(const InfInt& ii)
{
    value = ii.value;
    return *this;
}

InfInt InfInt::operator+(const InfInt& ii) const
{
    InfInt lhs = *this, rhs = ii, result;
    int carry = 0, temp = 0;
    result.value.clear();
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = lhs.size() - 1; i >= 0; --i){
        temp = (int) lhs.value[i] + (int) rhs.value[i] + carry;
        result.value.push_front(temp & 1); // if the result of addition is 01, we capture 0[1] bit
        carry = temp >> 1; // the the result of addition is 01, we capture [0]1 bit (01) >> 1 = (0)
    }
    if(carry)
        result.value.push_front(true);
    return result;
}

InfInt InfInt::operator-(const InfInt& ii) const
{   
    if(*this < ii)
        return (InfInt) 0;
    InfInt lhs = *this, rhs = ii, result;
    int borrow = 0, temp = 0;
    result.value.clear();
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = lhs.size() - 1; i >= 0; --i){
        temp = (int) lhs.value[i] - (int) rhs.value[i] - borrow;
        if(temp >= 0){
            result.value.push_front(temp);
            borrow = 0;
        } else {
            result.value.push_front(temp + 2);
            borrow = 1;
        }
    }
    return result.truncate_front_zeros();
}

InfInt InfInt::operator*(const InfInt& ii) const // LongMul
{
    InfInt lhs = *this, rhs = ii, result, temp;
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = lhs.size() - 1; i >= 0; --i){
        if(rhs.value[i])
            result = result + (lhs << ( lhs.size() - 1 - i)); 
    }
    return result;
} 

InfInt InfInt::operator/(const InfInt& ii) const // LongDiv1
{
    if(ii == 0)
        throw std::invalid_argument("cannot divide by 0");
    int k = ii.size() - 1;
    InfInt R = *this, Q = 0;
    while(R >= ii){
        int t = R.size() - 1;
        InfInt C = ii << (t - k);
        if(R < C){
            t = t - 1;
            C = ii << (t - k);
        }
        R = R - C;
        Q = Q + (1 << (t - k));
    }
    return Q;
}

InfInt InfInt::operator%(const InfInt& ii) const
{
    if(ii == 0)
        throw std::invalid_argument("cannot divide by 0");
    int k = ii.size() - 1;
    InfInt R = *this, Q = 0;
    while(R >= ii){
        int t = R.size() - 1;
        InfInt C = ii << (t - k);
        if(R < C){
            t = t - 1;
            C = ii << (t - k);
        }
        R = R - C;
//        Q = Q + (1 << (t - k)); we don't need Q, just R
    }
    return R;
}

/* relational operators */

bool InfInt::operator==(const InfInt& ii) const
{
    if(this->size() != ii.size())
        return false;
    for(int i = this->size() - 1; i >= 0; --i){
        if(this->value[i] != ii.value[i]){
            return false;
        }
    }
    return true;
}

bool InfInt::operator!=(const InfInt& ii) const
{
    return !(*this == ii);
}

bool InfInt::operator<(const InfInt& ii) const
{
    if(size() > ii.size())
        return false;
    if(size() < ii.size())
        return true;
    for(int i = 0; i < size(); ++i){
        if(value[i] != ii.value[i])
            return ii.value[i]; // if we found different bits, it's either (1,[0]) (first is bigger), or (0,[1]) (second is bigger), so we return only second bit
    }
    return false; // if both ints are equal, the size will be the same and we won't find different bits
}

bool InfInt::operator>(const InfInt& ii) const
{
    return ii < *this;
}

bool InfInt::operator>=(const InfInt& ii) const
{
    return !(*this < ii);   
}

bool InfInt::operator<=(const InfInt& ii) const
{
    return !(*this > ii);
}

/* bitwise operators */

InfInt InfInt::operator~() const
{
    InfInt result;
    for(int i = 0; i < size(); ++i)
        result.value.push_back(!value[i]);
    return result.truncate_front_zeros();
}

InfInt InfInt::operator&(const InfInt& ii) const
{
    InfInt lhs = *this, rhs = ii, result;
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = 0; i < lhs.size(); ++i)
        result.value.push_back(lhs.value[i] & rhs.value[i]);
    return result.truncate_front_zeros();
}

InfInt InfInt::operator|(const InfInt& ii) const
{
    InfInt lhs = *this, rhs = ii, result;
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = 0; i < lhs.size(); ++i)
        result.value.push_back(lhs.value[i] | rhs.value[i]);
    return result.truncate_front_zeros();
}

InfInt InfInt::operator^(const InfInt& ii) const
{
    InfInt lhs = *this, rhs = ii, result;
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0);// resizing ints to make sure they are the same size
    for(int i = 0; i < lhs.size(); ++i)
        result.value.push_back(lhs.value[i] ^ rhs.value[i]);
    return result.truncate_front_zeros();
}

InfInt InfInt::operator>>(const int& i) const
{
    InfInt result = *this;
    if(value.size() <= i)
        result.value.clear();
    else for(int j = 0; j < i; j++){
        result.value.pop_back();
    }
    return result;
};

InfInt& InfInt::operator>>=(const int& i)
{
    if(value.size() <= i)
        this->value.clear();
    else for(int j = 0; j < i; j++){
        this->value.pop_back();
    }
    return *this;
};

InfInt InfInt::operator<<(const int& i) const
{
    InfInt result = *this;
    for(int j = 0; j < i; ++j){
        result.value.push_back(0);
    }
    return result;
}

InfInt& InfInt::operator<<=(const int& i)
{
    for(int j = 0; j < i; ++j){
        this->value.push_back(0);
    }
    return *this;
}

/* size in bits */

inline size_t InfInt::size() const
{   
    return value.size();
};

/* power */

InfInt InfInt::to_power(const InfInt& ii) const //LongPower1
{
    InfInt C = 1, A = *this;
    for(int i = ii.size() - 1; i >= 0; --i){
        if(ii.value[i])
            C = C * A;
        A = A * A;
    }
    return C;
}

/* conversions */

std::string InfInt::to_string() const
{
    std::string result = "";
    for(int i = 0; i < size(); ++i)
        result += (value[i] ? "1" : "0");
    return result;
}

std::string InfInt::to_hex_string() const
{
    std::string result;
    size_t four_remainder = size() % 4;
    std::deque<bool>operational_value = value;
    if(four_remainder > 0)
        operational_value.insert(value.begin(), four_remainder, false);
    for(int i = 0; i < operational_value.size() - 3; i += 4){
        int window_decimal_value = (operational_value[i] * 8) + (operational_value[i+1] * 4) + (operational_value[i+2] * 2) + (operational_value[i+3] * 1);
        if(window_decimal_value < 10)
            result += std::to_string(window_decimal_value);
        else result += ('A' + window_decimal_value % 10);
    }
    return result;
}

int InfInt::to_int() const // NOTE: would be a good idea to implement exceptions 
{
    int result = 0;
    int base = 1;
    for(int i = size() - 1; i >= 0; --i){
        result += value[i] * base;
        base *= 2;
        if(i > 64) // on some 32bit machines int will most likely be 32 bit size, but this is good enough.
            break;
    }
    return result;
}

/* hex string support */

void InfInt::read_from_hex_string(const std::string& s)
{
    value.clear();
    int temp_int;
    for(int i = s.size() - 1; i >= 0; --i){
        if( ((int) s[i] > 47) && ((int) s[i] < 58) ) // 57th characters in ASCII Table is '9', so if 'a' is 9 or less, and 48th char is ASCII is '0', so if 'a' is 0 or greater
            temp_int = (int) s[i] - (int) '0'; // essentially we find the distance from '0' to 'a', which is '0'-'9', which basically gives us int number a that it represents
        else if( ((int) s[i]) > 64 && ((int) s[i] < 70) ) // checking if 'a' is bound by 'A' and 'F'
            temp_int =  10 + (int) s[i] - (int) 'A'; // once again, finding the distance from 'A' to char 'a' and + 10
        else throw std::invalid_argument("hexadecimal string contains unsupported characters");

        bool remainder = false; // modified int to binary converter (always runs 4 cycles, which is enough for our maximum 15)
        for(int j = 0; j < 4; ++j){
            remainder = temp_int % 2;
            temp_int /= 2;
            value.push_front(remainder);
        }
    }
    truncate_front_zeros();
}