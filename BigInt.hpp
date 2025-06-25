/*
    BigInt
    ------
    Arbitrary-sized integer class for C++.
    
    Version: 0.5.0-dev
    Released on: 05 October 2020 23:15 IST
    Author: Syed Faheel Ahmad (faheel@live.in)
    Project on GitHub: https://github.com/faheel/BigInt
    License: MIT
*/

/*
    ===========================================================================
    BigInt
    ===========================================================================
    Definition for the BigInt class.
*/

#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <iostream>
#include <string>
#include <tuple>

class BigInt {
    std::string value;
    char sign;
public:
    // Constructors:
    BigInt();
    BigInt(const BigInt&);
    BigInt(const long long&);
    BigInt(const std::string&);

    // Assignment operators:
    BigInt& operator=(const BigInt&);
    BigInt& operator=(const long long&);
    BigInt& operator=(const std::string&);

    // Unary arithmetic operators:
    BigInt operator+() const;
    BigInt operator-() const;

    // Binary arithmetic operators:
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;
    BigInt operator+(const long long&) const;
    BigInt operator-(const long long&) const;
    BigInt operator*(const long long&) const;
    BigInt operator/(const long long&) const;
    BigInt operator%(const long long&) const;
    BigInt operator+(const std::string&) const;
    BigInt operator-(const std::string&) const;
    BigInt operator*(const std::string&) const;
    BigInt operator/(const std::string&) const;
    BigInt operator%(const std::string&) const;

    // Arithmetic-assignment operators:
    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator+=(const long long&);
    BigInt& operator-=(const long long&);
    BigInt& operator*=(const long long&);
    BigInt& operator/=(const long long&);
    BigInt& operator%=(const long long&);
    BigInt& operator+=(const std::string&);
    BigInt& operator-=(const std::string&);
    BigInt& operator*=(const std::string&);
    BigInt& operator/=(const std::string&);
    BigInt& operator%=(const std::string&);

    // Increment and decrement operators:
    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);

    // Relational operators:
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const long long&) const;
    bool operator>(const long long&) const;
    bool operator<=(const long long&) const;
    bool operator>=(const long long&) const;
    bool operator==(const long long&) const;
    bool operator!=(const long long&) const;
    bool operator<(const std::string&) const;
    bool operator>(const std::string&) const;
    bool operator<=(const std::string&) const;
    bool operator>=(const std::string&) const;
    bool operator==(const std::string&) const;
    bool operator!=(const std::string&) const;

    // I/O stream operators:
    friend std::istream& operator>>(std::istream&, BigInt&);
    friend std::ostream& operator<<(std::ostream&, const BigInt&);

    // Conversion functions:
    std::string to_string() const;
    int to_int() const;
    long to_long() const;
    long long to_long_long() const;

    // Random number generating functions:
    friend BigInt big_random(size_t);
};

// Adding random
BigInt big_random(size_t num_digits);

// Utility function declarations
bool is_valid_number(const std::string& num);
void strip_leading_zeroes(std::string& num);
void add_leading_zeroes(std::string& num, size_t num_zeroes);
void add_trailing_zeroes(std::string& num, size_t num_zeroes);
std::tuple<std::string, std::string> get_larger_and_smaller(const std::string& num1, const std::string& num2);
bool is_power_of_10(const std::string& num);

// Math function declarations
BigInt abs(const BigInt& num);
BigInt big_pow10(size_t exp);
BigInt pow(const BigInt& base, int exp);
BigInt pow(const long long& base, int exp);
BigInt pow(const std::string& base, int exp);
BigInt sqrt(const BigInt& num);
BigInt gcd(const BigInt &num1, const BigInt &num2);
BigInt gcd(const BigInt& num1, const long long& num2);
BigInt gcd(const BigInt& num1, const std::string& num2);
BigInt gcd(const long long& num1, const BigInt& num2);
BigInt gcd(const std::string& num1, const BigInt& num2);
BigInt lcm(const BigInt& num1, const BigInt& num2);
BigInt lcm(const BigInt& num1, const long long& num2);
BigInt lcm(const BigInt& num1, const std::string& num2);
BigInt lcm(const long long& num1, const BigInt& num2);
BigInt lcm(const std::string& num1, const BigInt& num2);

//ostream operator 
std::ostream& operator<<(std::ostream& out, const BigInt& num);
#endif // BIG_INT_HPP

