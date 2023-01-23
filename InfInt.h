#pragma once

#include <deque>
#include <string>

class InfInt
{

public:
    /* constructors */
    InfInt();
    InfInt(const std::string& s);
    InfInt(int i);
    InfInt(const InfInt& ii);

    /* operators */                    // Lab 1 Functionality
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