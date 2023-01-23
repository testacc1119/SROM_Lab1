#pragma once

#include <deque>
#include <string>
#include <iostream> // just for exceptions

class InfInt
{

/////////////////////// Lab 2 functionality ///////////////////////

friend InfInt add_mod(const InfInt& a, const InfInt& b, const InfInt& n);

friend InfInt sub_mod(const InfInt& a, const InfInt& b, const InfInt& n);

friend InfInt mult_mod(const InfInt& a, const InfInt& b, const InfInt& n);

friend InfInt gcdEuclid(const InfInt& a, const InfInt& b);

friend InfInt lcmEuclid(const InfInt& a, const InfInt& b);

friend InfInt gcdBinary(InfInt a, InfInt b);

friend InfInt lcmBinary(const InfInt& a, const InfInt& b);

friend InfInt Mu(const InfInt& ii);

friend InfInt Barret_redc(const InfInt& x, const InfInt& n, const InfInt& m);

friend InfInt power_Barret_mod1(InfInt a, const InfInt& b, const InfInt& n);

friend InfInt power_Barret_mod2(const InfInt& a, const InfInt& b, const InfInt& n);

public:

/////////////////////// Lab 1 functionality ///////////////////////

    /* constructors */
    InfInt();
    InfInt(const std::string& s);
    InfInt(int i);
    InfInt(const InfInt& ii);

    /* operators */
    InfInt& operator=(const InfInt& ii);
    InfInt operator+(const InfInt& ii) const;
    InfInt operator-(const InfInt& ii) const;
    InfInt operator*(const InfInt& ii) const;
    InfInt operator/(const InfInt& ii) const;
    InfInt operator%(const InfInt& ii) const;

    /* relational operators */
    bool operator==(const InfInt& ii) const;
    bool operator!=(const InfInt& ii) const;
    bool operator<(const InfInt& ii) const;
    bool operator>(const InfInt& ii) const;
    bool operator<=(const InfInt& ii) const;
    bool operator>=(const InfInt& ii) const;

    /* bitwise operators */
    InfInt operator~() const;
    InfInt operator&(const InfInt& ii) const;
    InfInt operator|(const InfInt& ii) const;
    InfInt operator^(const InfInt& ii) const;
    InfInt operator>>(const int& i) const;
    InfInt& operator>>=(const int& i);
    InfInt operator<<(const int& i) const;
    InfInt& operator<<=(const int& i);

    /* size in bits */
    size_t size() const;

    /* power */
    InfInt to_power(const InfInt& ii) const;

    /* conversions */
    std::string to_string() const;
    std::string to_hex_string() const;
    int to_int() const;

    /* hex string support */
    void read_from_hex_string(const std::string& s);

private:
    /* helper functions to deal with different size numbers */
    void resize_front(const size_t& new_size, bool filler);
    InfInt truncate_front_zeros();

    // the oldest non-zero bit is always the first, so no reason for it

    std::deque<bool> value;
};

/////////////////////// Lab 1 functionality ///////////////////////

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
    if(ii == 2)
        return *this << 1;
    InfInt lhs = *this, rhs = ii, result, temp;
    lhs.size() > rhs.size() ? rhs.resize_front(lhs.size(), 0) : lhs.resize_front(rhs.size(), 0); // resizing ints to make sure they are the same size
    for(int i = lhs.size() - 1; i >= 0; --i){
        if(rhs.value[i])
            result = result + (lhs << ( lhs.size() - 1 - i)); 
    }
    return result.truncate_front_zeros();
} 

InfInt InfInt::operator/(const InfInt& ii) const // LongDiv1
{
    if(ii == 0)
        throw std::invalid_argument("cannot divide by 0");
    if(ii == 2)
        return *this >> 1;
    int k = ii.size() - 1;
    InfInt R = *this, Q = 0;
    while(R >= ii){
        int t = R.size() - 1;
        InfInt result = ii << (t - k);
        if(R < result){
            t = t - 1;
            result = ii << (t - k);
        }
        R = R - result;
        Q = Q + (1 << (t - k));
    }
    return Q;
}

InfInt InfInt::operator%(const InfInt& ii) const
{
    if(ii == 0)
        throw std::invalid_argument("cannot divide by 0");
    if(ii == 2)
        return (InfInt) value[size() - 1];
    int k = ii.size() - 1;
    InfInt R = *this, Q = 0;
    while(R >= ii){
        int t = R.size() - 1;
        InfInt result = ii << (t - k);
        if(R < result){
            t = t - 1;
            result = ii << (t - k);
        }
        R = R - result;
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

InfInt InfInt::to_power(const InfInt& ii) const // LongPower2
{
    InfInt result = 1;
    for(int i = 0; i < ii.size(); ++i){
        if(ii.value[i])
            result = result * *this;
        if(i != ii.size() - 1) 
            result = result * result;
    }
    return result;
}

/* 
InfInt InfInt::to_power(const InfInt& ii) const //LongPower1
{
    InfInt result = 1, A = *this;
    for(int i = ii.size() - 1; i >= 0; --i){
        if(ii.value[i])
            result = result * A;
        A = A * A;
    }
    return result;
}
 */

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

/////////////////////// Lab 2 functionality ///////////////////////

InfInt add_mod(const InfInt& a, const InfInt& b, const InfInt& n)
{
    InfInt mu = Mu(n);
    return Barret_redc( Barret_redc(a, n, mu) + Barret_redc(b , n, mu), n, mu);
};

InfInt sub_mod(const InfInt& a, const InfInt& b, const InfInt& n)
{
    InfInt mu = Mu(n);
    return Barret_redc( Barret_redc(a, n, mu) - Barret_redc(b , n, mu), n, mu);
};

InfInt mult_mod(const InfInt& a, const InfInt& b, const InfInt& n)
{
    InfInt mu = Mu(n);
    return Barret_redc( Barret_redc(a, n, mu) * Barret_redc(b , n, mu), n, mu);
};

InfInt gcdEuclid(const InfInt& a, const InfInt& b)
{
    return (b == 0) ? a : gcdEuclid(b, a % b);
};

InfInt lcmEuclid(const InfInt& a, const InfInt& b)
{
    return (a * b) / gcdEuclid(a, b);
};

InfInt gcdBinary(InfInt a, InfInt b)
{
    InfInt d = 1;
    while( (a % 2 == 0) && (b % 2 == 0)){
        a = a / 2;
        b = b / 2;
        d = d * 2;
    }
    
    InfInt temp;

    while(a % 2 == 0)
        a = a / 2;
    while(b != 0){
        while (b % 2 == 0)
            b = b / 2;
        if(a < b)
            b = b - a;
        else {
            temp = a;
            a = b;
            b = temp - b;
        }
    }
    return d * a;
};

InfInt lcmBinary(const InfInt& a, const InfInt& b)
{
    return (a * b) / gcdBinary(a, b);
}

InfInt Mu(const InfInt& ii)
{
    InfInt mu = 1 << (ii.size() * 2); // 2^(2k)
    mu = mu / ii; // / n
    return mu;
};

InfInt Barret_redc(const InfInt& x, const InfInt& n, const InfInt& mu)
{
    InfInt r, q;
    q = x >> (n.size() - 1);
    q = q * mu;
    q = q >> (n.size() + 1);
    r = x - q * n;
    while(r >= n){
        r = r - n;
    }
    return r.truncate_front_zeros();
}

InfInt power_Barret_mod1(InfInt a, const InfInt& b, const InfInt& n) //LongPower1
{
    InfInt result = 1, mu = Mu(n);
    for(int i = b.size() - 1; i >= 0; --i){
        if(b.value[i])
            result = Barret_redc(result * a, n, mu);
        a = Barret_redc(a * a, n, mu);
    }
    return result.truncate_front_zeros();
}

InfInt power_Barret_mod2(const InfInt& _a, const InfInt& b, const InfInt& n) // LongPower2
{
    InfInt a = _a % n, m = Mu(n), result = 1;
    for(int i = 0; i < b.size() - 1; ++i){
        if(b.value[i]) 
            result = Barret_redc(result * a, n, m);
        if(i != 0) 
            result = Barret_redc(result * result, n, m);
    }
    return result;
};