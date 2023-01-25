#pragma once

#include <vector>
#include <string>
#include <iostream> // just for exceptions

////////////////////////// Lab 3 //////////////////////////

#define field_pow 251 // 7 variant: 251; usual macros syntax


/* bitwise operators on vector<bool> */

static std::vector<bool> shiftL(const std::vector<bool>& v, const int& shift);
static std::vector<bool> shiftR(const std::vector<bool>& v, const int& shift);

// return v mod p, where p is polynomial generator
void mod_p(const std::vector<bool>& v);

// relational operators for BigIntPol's vectors
bool operator<(const std::vector<bool>& lhs, const std::vector<bool>& rhs);
bool operator>(const std::vector<bool>& lhs, const std::vector<bool>& rhs);
bool operator<=(const std::vector<bool>& lhs, const std::vector<bool>& rhs);
bool operator>=(const std::vector<bool>& lhs, const std::vector<bool>& rhs);

class BigIntPol
{

    friend std::vector<bool> get_double_sized(const std::vector<bool>&); // needed for multiplication and taking by modulo p
    friend std::vector<bool> discard_to_normal(const std::vector<bool>&); // the same thing


public:

    /* constructors */
    BigIntPol();
    BigIntPol(const std::string& s);
    BigIntPol(const std::vector<bool>& v); // combined with get_double_sized and discard_to_normal this enables us to make operations 
    BigIntPol(const BigIntPol& ii);

    /* operators */
    BigIntPol& operator=(const BigIntPol& ii);
    BigIntPol operator+(const BigIntPol& ii) const;
    BigIntPol operator*(const BigIntPol& ii) const;

    /* relational operators */
    bool operator==(const BigIntPol& ii) const;
    bool operator!=(const BigIntPol& ii) const;
    bool operator<(const BigIntPol& ii) const;
    bool operator>(const BigIntPol& ii) const;
    bool operator<=(const BigIntPol& ii) const;
    bool operator>=(const BigIntPol& ii) const;

    /* bitwise operators */
    BigIntPol operator~() const;
    BigIntPol operator&(const BigIntPol& ii) const;
    BigIntPol operator|(const BigIntPol& ii) const;
    BigIntPol operator^(const BigIntPol& ii) const;

    /* size in bits */
    size_t size() const;

    /* square */
    BigIntPol square() const;

    /* inverse */
    /* BigIntPol inverse() const; Horner too inefficient */

    /* power */
    //BigIntPol to_power(const BigIntPol& ii) const;

    /* conversions */
    std::string to_string() const;
    std::string to_tidy_string() const;

private:

    std::vector<bool> value;
};


std::vector<bool> plus(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    std::vector<bool> result(lhs.size(), false);
    for(int i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] ^ rhs[i];
    return result;
}

std::vector<bool> shiftL(const std::vector<bool>& ii, const int& shift)
{
    std::vector<bool> result = ii;
    for(int i = 0; i < shift; ++i){
        for(int i = ii.size() - 1; i >= 1; --i){
            result [i] = result[i - 1];
        }
        result[0] = false;
    }
    return result;
}

void mod_p(std::vector<bool>& value)
{
    std::vector<bool> p (2*field_pow, false);
    p[0] = p[1] = p[4] = p[14] = p[field_pow] = 1; // x^251 + x^14 + x^4 + x + 1
    //x^0  x^1    x^4    x^14          x^251
    /* if(value < p)
        return; */
    for(int i = 2*field_pow - 1; i >= field_pow; --i){
        // value = value ^ (p << 2*field_pow - field_pow)
        if(value[i])
            value = plus(value, shiftL(p, i - field_pow)); 
    }
}
std::vector<bool> get_double_sized(const std::vector<bool>& v)
{
    std::vector<bool> result(2*field_pow, false);
    for(int i = 0; i < field_pow; ++i)
        result[i] = v[i];
    return result;
}

std::vector<bool> discard_to_normal(const std::vector<bool>& v)
{
    std::vector<bool> result(field_pow, false);
    for(int i = 0; i < field_pow; ++i)
        result[i] = v[i];
    return result;
}

bool operator<(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    if(lhs.size() > rhs.size())
        return false;
    if(lhs.size() > rhs.size())
        return true;
    else for(int i = lhs.size() - 1; i >= 0; --i){
        if(lhs[i] != rhs[i])
            return rhs[i]; // if we found different bits, it's either (1,[0]) (first is bigger), or (0,[1]) (second is bigger), so we return only second bit
    }
    return false; // if both ints are equal, the size will be the same and we won't find different bits
}

bool operator>(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    return rhs < lhs;
}

bool operator>=(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    return !(lhs < rhs);   
}

bool operator<=(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    return !(lhs > rhs);
}

/* constructors */

BigIntPol::BigIntPol() : value(field_pow, false) {};

BigIntPol::BigIntPol(const std::string& s)
{
    /* if(s.size() >= field_pow){
        std::vector<bool> v (field_pow, false);
        value = v;
        return;
    };
 */
    value = std::vector<bool>(field_pow, false);
    int j = 0;
    for(int i = s.size() - 1; i >= 0; --i, j++){
        value[j] = (s[i] == '1'); // number are represented "101100..00"
    }
}

BigIntPol::BigIntPol(const std::vector<bool>& v)
{
    value = v;
}

BigIntPol::BigIntPol(const BigIntPol& ii)
{
    this->value = ii.value;
}

/* operators */

BigIntPol& BigIntPol::operator=(const BigIntPol& ii)
{
    value = ii.value;
    return *this;
}

BigIntPol BigIntPol::operator+(const BigIntPol& ii) const
{
    BigIntPol result;
    for(int i = 0; i < field_pow; ++i){
        result.value[i] =  (this->value[i]) ^ (ii.value[i]);
    }
    return result;
}

BigIntPol BigIntPol::operator*(const BigIntPol& ii) const // LongMul
{
    std::vector<bool> double_sized_result (2*field_pow, false);
    std::vector<bool> double_sized_lhs = get_double_sized(this->value);
    std::vector<bool> double_sized_rhs = get_double_sized(ii.value);
    for(int i = 0; i < 2*field_pow; ++i, double_sized_lhs = shiftL(double_sized_lhs, 1))
    {
        if(double_sized_rhs[i])
            double_sized_result = plus(double_sized_result, double_sized_lhs);
        /*  double_sized_result = plus(double_sized_result, shiftL(double_sized_lhs, i));  the same thing*/
    }

    mod_p(double_sized_result);
    std::vector<bool> normal_sized_result = discard_to_normal(double_sized_result);

    return (BigIntPol) normal_sized_result;
} 

/* relational operators */

bool BigIntPol::operator==(const BigIntPol& ii) const
{
    for(int i = 0; i < field_pow; ++i){
        if(this->value[i] != ii.value[i]){
            return false;
        }
    }
    return true;
}

bool BigIntPol::operator!=(const BigIntPol& ii) const
{
    return !(*this == ii);
}

bool BigIntPol::operator<(const BigIntPol& ii) const
{
    for(int i = field_pow - 1; i >= 0; --i){
        if(value[i] != ii.value[i])
            return ii.value[i]; // if we found different bits, it's either (1,[0]) (first is bigger), or (0,[1]) (second is bigger), so we return only second bit
    }
    return false; // if both ints are equal, the size will be the same and we won't find different bits
}

bool BigIntPol::operator>(const BigIntPol& ii) const
{
    return ii < *this;
}

bool BigIntPol::operator>=(const BigIntPol& ii) const
{
    return !(*this < ii);   
}

bool BigIntPol::operator<=(const BigIntPol& ii) const
{
    return !(*this > ii);
}

/* bitwise operators */

BigIntPol BigIntPol::operator~() const
{
    BigIntPol result;
    for(int i = 0; i < field_pow; ++i)
        result.value[i] = !value[i];
    return result;
}

BigIntPol BigIntPol::operator&(const BigIntPol& ii) const
{
    BigIntPol result;
    for(int i = 0; i < field_pow; ++i)
        result.value[i] = value[i] & ii.value[i];
    return result;
}

BigIntPol BigIntPol::operator|(const BigIntPol& ii) const
{
    BigIntPol result;
    for(int i = 0; i < field_pow; ++i)
        result.value[i] = value[i] | ii.value[i];
    return result;
}

BigIntPol BigIntPol::operator^(const BigIntPol& ii) const
{
    BigIntPol result;
    for(int i = 0; i < field_pow; ++i)
        result.value[i] = value[i] ^ ii.value[i];
    return result;
}

/* size in bits */

inline size_t BigIntPol::size() const
{   
    return value.size();
}

/* square */

BigIntPol BigIntPol::square() const
{
    std::vector<bool> double_sized_result = get_double_sized(this->value);
    for(int i = 0; i < field_pow; ++i)
        double_sized_result[i * 2] = value[i];
    mod_p(double_sized_result);
    std::vector<bool> normal_sized_result = discard_to_normal(double_sized_result);

    return (BigIntPol) normal_sized_result;
}

/* inverse */

/* BigIntPol BigIntPol::inverse() const // Horner
{
    BigIntPol result("1"); // our multiplication identity
    for(int i = 0; i < field_pow - 1; ++i){
        result = result * *this;
        result = result.square();
    }
    return result;
} */

/* conversions */

std::string BigIntPol::to_string() const
{
    std::string result;
    for(int i = size() - 1; i >= 0; --i)
        result += (value[i] ? "1" : "0");
    return result;
}

std::string BigIntPol::to_tidy_string() const
{
    std::string result;
    bool number_started = false;
    for(int i = size() - 1; i >= 0; --i){
        if(value[i] == 1)
            if(number_started == false){
                number_started = true;
                result += "1";
            }
            else result += "1";
        else {
            if(number_started == false){}
            else result += "0";
        }
    }
    return result;
}