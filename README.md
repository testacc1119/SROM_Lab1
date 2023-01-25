# SROM Дигас Богдан ФІ-03
## Lab 1
### Implemented all these features:

* Resizable bool array based on the std::deque chosen because of it's ability to push_front() and push_back(), essential for efficiency on a larger scale

* All bitwise operators, applied to bool type (&, |, ^, ~(!)) and bitshifts (>>, <<)

* All relational operators (>, <, >=, <=, !=, ==)

* Addition, subtraction, multiplication, division, finding the remainder and pow

* Binary string, int (int to string is easily implemented by the user) and hexadecimal string conversions

* sidenote: oldest positive bit is always the first one, so no need to implement search for it

## Lab 2
### Implemented all these features:

* Addition, subtraction, multiplication, power using Barret reduction

* gcd, lcm by Euclidian and binary methods 

## Lab 3
### Implemented all these features:

* Static size bool array based on the std::vector chosen because it's optimization in c++ to allocate each individual bool in the vector of bools one bit, not one byte

* Addition, subtraction, multiplication, square in a field(2) with polynomial basis (Variant 7: p = x^251 + x^14 + x^4 + x + 1)

* All bitwise operators, applied to bool type (&, |, ^, ~(!)) and bitshift left (<<) (the only bitshift used)

* All relational operators (>, <, >=, <=, !=, ==)

* Conversion to string and tidy_string (without useless zeros at the end of the number)

*sidenote: inverse by Horner is too inefficient, better leave it alone