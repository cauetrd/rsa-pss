#include "BigInt.hpp"

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


#include <iostream>


/*
    ===========================================================================
    Utility functions
    ===========================================================================
*/


#include <tuple>


/*
    is_valid_number
    ---------------
    Checks whether the given string is a valid integer.
*/

bool is_valid_number(const std::string& num) {
    for (char digit : num)
        if (digit < '0' || digit > '9')
            return false;

    return true;
}


/*
    strip_leading_zeroes
    --------------------
    Strip the leading zeroes from a number represented as a string.
*/

void strip_leading_zeroes(std::string& num) {
    size_t i;
    for (i = 0; i < num.size(); i++)
        if (num[i] != '0')
            break;

    if (i == num.size())
        num = "0";
    else
        num = num.substr(i);
}


/*
    add_leading_zeroes
    ------------------
    Adds a given number of leading zeroes to a string-represented integer `num`.
*/

void add_leading_zeroes(std::string& num, size_t num_zeroes) {
    num = std::string(num_zeroes, '0') + num;
}


/*
    add_trailing_zeroes
    -------------------
    Adds a given number of trailing zeroes to a string-represented integer `num`.
*/

void add_trailing_zeroes(std::string& num, size_t num_zeroes) {
    num += std::string(num_zeroes, '0');
}


/*
    get_larger_and_smaller
    ----------------------
    Identifies the given string-represented integers as `larger` and `smaller`,
    padding the smaller number with leading zeroes to make it equal in length to
    the larger number.
*/

std::tuple<std::string, std::string> get_larger_and_smaller(const std::string& num1,
        const std::string& num2) {
    std::string larger, smaller;
    if (num1.size() > num2.size() ||
        (num1.size() == num2.size() && num1 > num2)) {
        larger = num1;
        smaller = num2;
    }
    else {
        larger = num2;
        smaller = num1;
    }

    // pad the smaller number with zeroes
    add_leading_zeroes(smaller, larger.size() - smaller.size());

    return std::make_tuple(larger, smaller);
}


/*
    is_power_of_10
    ----------------------
    Checks whether a string-represented integer is a power of 10.
*/

bool is_power_of_10(const std::string& num){
    if (num[0] != '1')
        return false;
    for (size_t i = 1; i < num.size(); i++)
        if (num[i] != '0')
            return false;

    return true;    // first digit is 1 and the following digits are all 0
}



/*
    ===========================================================================
    Random number generating functions for BigInt
    ===========================================================================
*/


#include <random>
#include <climits>


// when the number of digits are not specified, a random value is used for it
// which is kept below the following:
const size_t MAX_RANDOM_LENGTH = 1000;


/*
    big_random (num_digits)
    -----------------------
    Returns a random BigInt with a specific number of digits.
*/

BigInt big_random(size_t num_digits = 0) {
    std::random_device rand_generator;      // true random number generator

    if (num_digits == 0)    // the number of digits were not specified
        // use a random number for it:
        num_digits = 1 + rand_generator() % MAX_RANDOM_LENGTH;

    BigInt big_rand;
    big_rand.value = "";    // clear value to append digits

    // ensure that the first digit is non-zero
    big_rand.value += std::to_string(1 + rand_generator() % 9);

    while (big_rand.value.size() < num_digits)
        big_rand.value += std::to_string(rand_generator());
    if (big_rand.value.size() != num_digits)
        big_rand.value.erase(num_digits);   // erase extra digits

    return big_rand;
}




/*
    ===========================================================================
    Constructors
    ===========================================================================
*/




/*
    Default constructor
    -------------------
*/

BigInt::BigInt() {
    value = "0";
    sign = '+';
}


/*
    Copy constructor
    ----------------
*/

BigInt::BigInt(const BigInt& num) {
    value = num.value;
    sign = num.sign;
}


/*
    Integer to BigInt
    -----------------
*/

BigInt::BigInt(const long long& num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}


/*
    String to BigInt
    ----------------
*/

BigInt::BigInt(const std::string& num) {
    if (num[0] == '+' || num[0] == '-') {     // check for sign
        std::string magnitude = num.substr(1);
        if (is_valid_number(magnitude)) {
            value = magnitude;
            sign = num[0];
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    else {      // if no sign is specified
        if (is_valid_number(num)) {
            value = num;
            sign = '+';    // positive by default
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    strip_leading_zeroes(value);
}


/*
    ===========================================================================
    Conversion functions for BigInt
    ===========================================================================
*/



/*
    to_string
    ---------
    Converts a BigInt to a string.
*/

std::string BigInt::to_string() const {
    // prefix with sign if negative
    return this->sign == '-' ? "-" + this->value : this->value;
}


/*
    to_int
    ------
    Converts a BigInt to an int.
    NOTE: If the BigInt is out of range of an int, stoi() will throw an
    out_of_range exception.
*/

int BigInt::to_int() const {
    return std::stoi(this->to_string());
}


/*
    to_long
    -------
    Converts a BigInt to a long int.
    NOTE: If the BigInt is out of range of a long int, stol() will throw an
    out_of_range exception.
*/

long BigInt::to_long() const {
    return std::stol(this->to_string());
}


/*
    to_long_long
    ------------
    Converts a BigInt to a long long int.
    NOTE: If the BigInt is out of range of a long long int, stoll() will throw
    an out_of_range exception.
*/

long long BigInt::to_long_long() const {
    return std::stoll(this->to_string());
}



/*
    ===========================================================================
    Assignment operators
    ===========================================================================
*/




/*
    BigInt = BigInt
    ---------------
*/

BigInt& BigInt::operator=(const BigInt& num) {
    value = num.value;
    sign = num.sign;

    return *this;
}


/*
    BigInt = Integer
    ----------------
*/

BigInt& BigInt::operator=(const long long& num) {
    BigInt temp(num);
    value = temp.value;
    sign = temp.sign;

    return *this;
}


/*
    BigInt = String
    ---------------
*/

BigInt& BigInt::operator=(const std::string& num) {
    BigInt temp(num);
    value = temp.value;
    sign = temp.sign;

    return *this;
}



/*
    ===========================================================================
    Unary arithmetic operators
    ===========================================================================
*/



/*
    +BigInt
    -------
    Returns the value of a BigInt.
    NOTE: This function does not return the absolute value. To get the absolute
    value of a BigInt, use the `abs` function.
*/

BigInt BigInt::operator+() const {
    return *this;
}


/*
    -BigInt
    -------
    Returns the negative of a BigInt.
*/

BigInt BigInt::operator-() const {
    BigInt temp;

    temp.value = value;
    if (value != "0") {
        if (sign == '+')
            temp.sign = '-';
        else
            temp.sign = '+';
    }

    return temp;
}



/*
    ===========================================================================
    Relational operators
    ===========================================================================
    All operators depend on the '<' and/or '==' operator(s).
*/


/*
    BigInt == BigInt
    ----------------
*/

bool BigInt::operator==(const BigInt& num) const {
    return (sign == num.sign) && (value == num.value);
}


/*
    BigInt != BigInt
    ----------------
*/

bool BigInt::operator!=(const BigInt& num) const {
    return !(*this == num);
}


/*
    BigInt < BigInt
    ---------------
*/

bool BigInt::operator<(const BigInt& num) const {
    if (sign == num.sign) {
        if (sign == '+') {
            if (value.length() == num.value.length())
                return value < num.value;
            else
                return value.length() < num.value.length();
        }
        else
            return -(*this) > -num;
    }
    else
        return sign == '-';
}


/*
    BigInt > BigInt
    ---------------
*/

bool BigInt::operator>(const BigInt& num) const {
    return !((*this < num) || (*this == num));
}


/*
    BigInt <= BigInt
    ----------------
*/

bool BigInt::operator<=(const BigInt& num) const {
    return (*this < num) || (*this == num);
}


/*
    BigInt >= BigInt
    ----------------
*/

bool BigInt::operator>=(const BigInt& num) const {
    return !(*this < num);
}


/*
    BigInt == Integer
    -----------------
*/

bool BigInt::operator==(const long long& num) const {
    return *this == BigInt(num);
}


/*
    Integer == BigInt
    -----------------
*/

bool operator==(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) == rhs;
}


/*
    BigInt != Integer
    -----------------
*/

bool BigInt::operator!=(const long long& num) const {
    return !(*this == BigInt(num));
}


/*
    Integer != BigInt
    -----------------
*/

bool operator!=(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) != rhs;
}


/*
    BigInt < Integer
    ----------------
*/

bool BigInt::operator<(const long long& num) const {
    return *this < BigInt(num);
}


/*
    Integer < BigInt
    ----------------
*/

bool operator<(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) < rhs;
}


/*
    BigInt > Integer
    ----------------
*/

bool BigInt::operator>(const long long& num) const {
    return *this > BigInt(num);
}


/*
    Integer > BigInt
    ----------------
*/

bool operator>(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) > rhs;
}


/*
    BigInt <= Integer
    -----------------
*/

bool BigInt::operator<=(const long long& num) const {
    return !(*this > BigInt(num));
}


/*
    Integer <= BigInt
    -----------------
*/

bool operator<=(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) <= rhs;
}


/*
    BigInt >= Integer
    -----------------
*/

bool BigInt::operator>=(const long long& num) const {
    return !(*this < BigInt(num));
}


/*
    Integer >= BigInt
    -----------------
*/

bool operator>=(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) >= rhs;
}


/*
    BigInt == String
    ----------------
*/

bool BigInt::operator==(const std::string& num) const {
    return *this == BigInt(num);
}


/*
    String == BigInt
    ----------------
*/

bool operator==(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) == rhs;
}


/*
    BigInt != String
    ----------------
*/

bool BigInt::operator!=(const std::string& num) const {
    return !(*this == BigInt(num));
}


/*
    String != BigInt
    ----------------
*/

bool operator!=(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) != rhs;
}


/*
    BigInt < String
    ---------------
*/

bool BigInt::operator<(const std::string& num) const {
    return *this < BigInt(num);
}


/*
    String < BigInt
    ---------------
*/

bool operator<(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) < rhs;
}


/*
    BigInt > String
    ---------------
*/

bool BigInt::operator>(const std::string& num) const {
    return *this > BigInt(num);
}


/*
    String > BigInt
    ---------------
*/

bool operator>(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) > rhs;
}


/*
    BigInt <= String
    ----------------
*/

bool BigInt::operator<=(const std::string& num) const {
    return !(*this > BigInt(num));
}


/*
    String <= BigInt
    ----------------
*/

bool operator<=(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) <= rhs;
}


/*
    BigInt >= String
    ----------------
*/

bool BigInt::operator>=(const std::string& num) const {
    return !(*this < BigInt(num));
}


/*
    String >= BigInt
    ----------------
*/

bool operator>=(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) >= rhs;
}



/*
    ===========================================================================
    Math functions for BigInt
    ===========================================================================
*/

#include <string>



/*
    abs
    ---
    Returns the absolute value of a BigInt.
*/

BigInt abs(const BigInt& num) {
    return num < 0 ? -num : num;
}


/*
    big_pow10
    ---------
    Returns a BigInt equal to 10^exp.
    NOTE: exponent should be a non-negative integer.
*/

BigInt big_pow10(size_t exp) {
    return BigInt("1" + std::string(exp, '0'));
}


/*
    pow (BigInt)
    ------------
    Returns a BigInt equal to base^exp.
*/

BigInt pow(const BigInt& base, int exp) {
    if (exp < 0) {
        if (base == 0)
            throw std::logic_error("Cannot divide by zero");
        return abs(base) == 1 ? base : 0;
    }
    if (exp == 0) {
        if (base == 0)
            throw std::logic_error("Zero cannot be raised to zero");
        return 1;
    }

    BigInt result = base, result_odd = 1;
    while (exp > 1) {
        if (exp % 2)
            result_odd *= result;
        result *= result;
        exp /= 2;
    }

    return result * result_odd;
}


/*
    pow (Integer)
    -------------
    Returns a BigInt equal to base^exp.
*/

BigInt pow(const long long& base, int exp) {
    return pow(BigInt(base), exp);
}


/*
    pow (String)
    ------------
    Returns a BigInt equal to base^exp.
*/

BigInt pow(const std::string& base, int exp) {
    return pow(BigInt(base), exp);

}


/*
    sqrt
    ----
    Returns the positive integer square root of a BigInt using Newton's method.
    NOTE: the input must be non-negative.
*/

BigInt sqrt(const BigInt& num) {
    if (num < 0)
        throw std::invalid_argument("Cannot compute square root of a negative integer");

    // Optimisations for small inputs:
    if (num == 0)
        return 0;
    else if (num < 4)
        return 1;
    else if (num < 9)
        return 2;
    else if (num < 16)
        return 3;

    BigInt sqrt_prev = -1;
    // The value for `sqrt_current` is chosen close to that of the actual
    // square root.
    // Since a number's square root has at least one less than half as many
    // digits as the number,
    //     sqrt_current = 10^(half_the_digits_in_num - 1)
    BigInt sqrt_current = big_pow10(num.to_string().size() / 2 - 1);

    while (abs(sqrt_current - sqrt_prev) > 1) {
        sqrt_prev = sqrt_current;
        sqrt_current = (num / sqrt_prev + sqrt_prev) / 2;
    }

    return sqrt_current;
}


/*
    gcd(BigInt, BigInt)
    -------------------
    Returns the greatest common divisor (GCD, a.k.a. HCF) of two BigInts using
    Euclid's algorithm.
*/

BigInt gcd(const BigInt &num1, const BigInt &num2){
    BigInt abs_num1 = abs(num1);
    BigInt abs_num2 = abs(num2);

    // base cases:
    if (abs_num2 == 0)
        return abs_num1;    // gcd(a, 0) = |a|
    if (abs_num1 == 0)
        return abs_num2;    // gcd(0, a) = |a|

    BigInt remainder = abs_num2;
    while (remainder != 0) {
        remainder = abs_num1 % abs_num2;
        abs_num1 = abs_num2;    // previous remainder
        abs_num2 = remainder;   // current remainder
    }

    return abs_num1;
}


/*
    gcd(BigInt, Integer)
    --------------------
*/

BigInt gcd(const BigInt& num1, const long long& num2){
    return gcd(num1, BigInt(num2));
}


/*
    gcd(BigInt, String)
    -------------------
*/

BigInt gcd(const BigInt& num1, const std::string& num2){
    return gcd(num1, BigInt(num2));
}


/*
    gcd(Integer, BigInt)
    --------------------
*/

BigInt gcd(const long long& num1, const BigInt& num2){
    return gcd(BigInt(num1), num2);
}


/*
    gcd(String, BigInt)
    -------------------
*/

BigInt gcd(const std::string& num1, const BigInt& num2){
    return gcd(BigInt(num1), num2);
}


/*
    lcm(BigInt, BigInt)
    -------------------
    Returns the least common multiple (LCM) of two BigInts.
*/

BigInt lcm(const BigInt& num1, const BigInt& num2) {
    if (num1 == 0 || num2 == 0)
        return 0;

    return abs(num1 * num2) / gcd(num1, num2);
}


/*
    lcm(BigInt, Integer)
    --------------------
*/

BigInt lcm(const BigInt& num1, const long long& num2){
    return lcm(num1, BigInt(num2));
}


/*
    lcm(BigInt, String)
    -------------------
*/

BigInt lcm(const BigInt& num1, const std::string& num2){
    return lcm(num1, BigInt(num2));
}


/*
    lcm(Integer, BigInt)
    --------------------
*/

BigInt lcm(const long long& num1, const BigInt& num2){
    return lcm(BigInt(num1), num2);
}


/*
    lcm(String, BigInt)
    -------------------
*/

BigInt lcm(const std::string& num1, const BigInt& num2){
    return lcm(BigInt(num1), num2);
}




/*
    ===========================================================================
    Binary arithmetic operators
    ===========================================================================
*/

#include <climits>
#include <cmath>
#include <string>


const long long FLOOR_SQRT_LLONG_MAX = 3037000499;


/*
    BigInt + BigInt
    ---------------
    The operand on the RHS of the addition is `num`.
*/

BigInt BigInt::operator+(const BigInt& num) const {
    // if the operands are of opposite signs, perform subtraction
    if (this->sign == '+' && num.sign == '-') {
        BigInt rhs = num;
        rhs.sign = '+';
        return *this - rhs;
    }
    else if (this->sign == '-' && num.sign == '+') {
        BigInt lhs = *this;
        lhs.sign = '+';
        return -(lhs - num);
    }

    // identify the numbers as `larger` and `smaller`
    std::string larger, smaller;
    std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);

    BigInt result;      // the resultant sum
    result.value = "";  // the value is cleared as the digits will be appended
    short carry = 0, sum;
    // add the two values
    for (long i = larger.size() - 1; i >= 0; i--) {
        sum = larger[i] - '0' + smaller[i] - '0' + carry;
        result.value = std::to_string(sum % 10) + result.value;
        carry = sum / (short) 10;
    }
    if (carry)
        result.value = std::to_string(carry) + result.value;

    // if the operands are negative, the result is negative
    if (this->sign == '-' && result.value != "0")
        result.sign = '-';

    return result;
}


/*
    BigInt - BigInt
    ---------------
    The operand on the RHS of the subtraction is `num`.
*/

BigInt BigInt::operator-(const BigInt& num) const {
    // if the operands are of opposite signs, perform addition
    if (this->sign == '+' && num.sign == '-') {
        BigInt rhs = num;
        rhs.sign = '+';
        return *this + rhs;
    }
    else if (this->sign == '-' && num.sign == '+') {
        BigInt lhs = *this;
        lhs.sign = '+';
        return -(lhs + num);
    }

    BigInt result;      // the resultant difference
    // identify the numbers as `larger` and `smaller`
    std::string larger, smaller;
    if (abs(*this) > abs(num)) {
        larger = this->value;
        smaller = num.value;

        if (this->sign == '-')      // -larger - -smaller = -result
            result.sign = '-';
    }
    else {
        larger = num.value;
        smaller = this->value;

        if (num.sign == '+')        // smaller - larger = -result
            result.sign = '-';
    }
    // pad the smaller number with zeroes
    add_leading_zeroes(smaller, larger.size() - smaller.size());

    result.value = "";  // the value is cleared as the digits will be appended
    short difference;
    long i, j;
    // subtract the two values
    for (i = larger.size() - 1; i >= 0; i--) {
        difference = larger[i] - smaller[i];
        if (difference < 0) {
            for (j = i - 1; j >= 0; j--) {
                if (larger[j] != '0') {
                    larger[j]--;    // borrow from the j-th digit
                    break;
                }
            }
            j++;
            while (j != i) {
                larger[j] = '9';    // add the borrow and take away 1
                j++;
            }
            difference += 10;   // add the borrow
        }
        result.value = std::to_string(difference) + result.value;
    }
    strip_leading_zeroes(result.value);

    // if the result is 0, set its sign as +
    if (result.value == "0")
        result.sign = '+';

    return result;
}


/*
    BigInt * BigInt
    ---------------
    Computes the product of two BigInts using Karatsuba's algorithm.
    The operand on the RHS of the product is `num`.
*/

BigInt BigInt::operator*(const BigInt& num) const {
    if (*this == 0 || num == 0)
        return BigInt(0);
    if (*this == 1)
        return num;
    if (num == 1)
     return *this;

    BigInt product;
    if (abs(*this) <= FLOOR_SQRT_LLONG_MAX && abs(num) <= FLOOR_SQRT_LLONG_MAX)
        product = std::stoll(this->value) * std::stoll(num.value);
    else if (is_power_of_10(this->value)){ // if LHS is a power of 10 do optimised operation 
        product.value = num.value;
        product.value.append(this->value.begin() + 1, this->value.end());
    }
    else if (is_power_of_10(num.value)){ // if RHS is a power of 10 do optimised operation 
        product.value = this->value;
        product.value.append(num.value.begin() + 1, num.value.end());
    }
    else {
        // identify the numbers as `larger` and `smaller`
        std::string larger, smaller;
        std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);

        size_t half_length = larger.size() / 2;
        auto half_length_ceil = (size_t) ceil(larger.size() / 2.0);

        BigInt num1_high, num1_low;
        num1_high = larger.substr(0, half_length);
        num1_low = larger.substr(half_length);

        BigInt num2_high, num2_low;
        num2_high = smaller.substr(0, half_length);
        num2_low = smaller.substr(half_length);

        strip_leading_zeroes(num1_high.value);
        strip_leading_zeroes(num1_low.value);
        strip_leading_zeroes(num2_high.value);
        strip_leading_zeroes(num2_low.value);

        BigInt prod_high, prod_mid, prod_low;
        prod_high = num1_high * num2_high;
        prod_low = num1_low * num2_low;
        prod_mid = (num1_high + num1_low) * (num2_high + num2_low)
                   - prod_high - prod_low;

        add_trailing_zeroes(prod_high.value, 2 * half_length_ceil);
        add_trailing_zeroes(prod_mid.value, half_length_ceil);

        strip_leading_zeroes(prod_high.value);
        strip_leading_zeroes(prod_mid.value);
        strip_leading_zeroes(prod_low.value);

        product = prod_high + prod_mid + prod_low;
    }
    strip_leading_zeroes(product.value);

    if (this->sign == num.sign)
        product.sign = '+';
    else
        product.sign = '-';

    return product;
}


/*
    divide
    ------
    Helper function that returns the quotient and remainder on dividing the
    dividend by the divisor, when the divisor is 1 to 10 times the dividend.
*/

std::tuple<BigInt, BigInt> divide(const BigInt& dividend, const BigInt& divisor) {
    BigInt quotient, remainder, temp;

    temp = divisor;
    quotient = 1;
    while (temp < dividend) {
        quotient++;
        temp += divisor;
    }
    if (temp > dividend) {
        quotient--;
        remainder = dividend - (temp - divisor);
    }

    return std::make_tuple(quotient, remainder);
}


/*
    BigInt / BigInt
    ---------------
    Computes the quotient of two BigInts using the long-division method.
    The operand on the RHS of the division (the divisor) is `num`.
*/

BigInt BigInt::operator/(const BigInt& num) const {
    BigInt abs_dividend = abs(*this);
    BigInt abs_divisor = abs(num);

    if (num == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_dividend < abs_divisor)
        return BigInt(0);
    if (num == 1)
        return *this;
    if (num == -1)
        return -(*this);

    BigInt quotient;
    if (abs_dividend <= LLONG_MAX && abs_divisor <= LLONG_MAX)
        quotient = std::stoll(abs_dividend.value) / std::stoll(abs_divisor.value);
    else if (abs_dividend == abs_divisor)
        quotient = 1;
    else if (is_power_of_10(abs_divisor.value)) { // if divisor is a power of 10 do optimised calculation
        size_t digits_in_quotient = abs_dividend.value.size() - abs_divisor.value.size() + 1;
        quotient.value = abs_dividend.value.substr(0, digits_in_quotient);
    }
    else {
        quotient.value = "";    // the value is cleared as digits will be appended
        BigInt chunk, chunk_quotient, chunk_remainder;
        size_t chunk_index = 0;
        chunk_remainder.value = abs_dividend.value.substr(chunk_index, abs_divisor.value.size() - 1);
        chunk_index = abs_divisor.value.size() - 1;
        while (chunk_index < abs_dividend.value.size()) {
            chunk.value = chunk_remainder.value.append(1, abs_dividend.value[chunk_index]);
            chunk_index++;
            while (chunk < abs_divisor) {
                quotient.value += "0";
                if (chunk_index < abs_dividend.value.size()) {
                    chunk.value.append(1, abs_dividend.value[chunk_index]);
                    chunk_index++;
                }
                else
                    break;
            }
            if (chunk == abs_divisor) {
                quotient.value += "1";
                chunk_remainder = 0;
            }
            else if (chunk > abs_divisor) {
                strip_leading_zeroes(chunk.value);
                std::tie(chunk_quotient, chunk_remainder) = divide(chunk, abs_divisor);
                quotient.value += chunk_quotient.value;
            }
        }
    }
    strip_leading_zeroes(quotient.value);

    if (this->sign == num.sign)
        quotient.sign = '+';
    else
        quotient.sign = '-';

    return quotient;
}


/*
    BigInt % BigInt
    ---------------
    Computes the modulo (remainder on division) of two BigInts.
    The operand on the RHS of the modulo (the divisor) is `num`.
*/

BigInt BigInt::operator%(const BigInt& num) const {
    BigInt abs_dividend = abs(*this);
    BigInt abs_divisor = abs(num);

    if (abs_divisor == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_divisor == 1 || abs_divisor == abs_dividend)
        return BigInt(0);

    BigInt remainder;
    if (abs_dividend <= LLONG_MAX && abs_divisor <= LLONG_MAX)
        remainder = std::stoll(abs_dividend.value) % std::stoll(abs_divisor.value);
    else if (abs_dividend < abs_divisor)
        remainder = abs_dividend;
    else if (is_power_of_10(num.value)){ // if num is a power of 10 use optimised calculation
        size_t no_of_zeroes = num.value.size() - 1;
        remainder.value = abs_dividend.value.substr(abs_dividend.value.size() - no_of_zeroes);
    } 
    else {
        BigInt quotient = abs_dividend / abs_divisor;
        remainder = abs_dividend - quotient * abs_divisor;
    }
    strip_leading_zeroes(remainder.value);

    // remainder has the same sign as that of the dividend
    remainder.sign = this->sign;
    if (remainder.value == "0")     // except if its zero
        remainder.sign = '+';

    return remainder;
}


/*
    BigInt + Integer
    ----------------
*/

BigInt BigInt::operator+(const long long& num) const {
    return *this + BigInt(num);
}


/*
    Integer + BigInt
    ----------------
*/

BigInt operator+(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}


/*
    BigInt - Integer
    ----------------
*/

BigInt BigInt::operator-(const long long& num) const {
    return *this - BigInt(num);
}


/*
    Integer - BigInt
    ----------------
*/

BigInt operator-(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}


/*
    BigInt * Integer
    ----------------
*/

BigInt BigInt::operator*(const long long& num) const {
    return *this * BigInt(num);
}


/*
    Integer * BigInt
    ----------------
*/

BigInt operator*(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) * rhs;
}


/*
    BigInt / Integer
    ----------------
*/

BigInt BigInt::operator/(const long long& num) const {
    return *this / BigInt(num);
}


/*
    Integer / BigInt
    ----------------
*/

BigInt operator/(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) / rhs;
}


/*
    BigInt % Integer
    ----------------
*/

BigInt BigInt::operator%(const long long& num) const {
    return *this % BigInt(num);
}


/*
    Integer % BigInt
    ----------------
*/

BigInt operator%(const long long& lhs, const BigInt& rhs) {
    return BigInt(lhs) % rhs;
}


/*
    BigInt + String
    ---------------
*/

BigInt BigInt::operator+(const std::string& num) const {
    return *this + BigInt(num);
}


/*
    String + BigInt
    ---------------
*/

BigInt operator+(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}


/*
    BigInt - String
    ---------------
*/

BigInt BigInt::operator-(const std::string& num) const {
    return *this - BigInt(num);
}


/*
    String - BigInt
    ---------------
*/

BigInt operator-(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}


/*
    BigInt * String
    ---------------
*/

BigInt BigInt::operator*(const std::string& num) const {
    return *this * BigInt(num);
}


/*
    String * BigInt
    ---------------
*/

BigInt operator*(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) * rhs;
}


/*
    BigInt / String
    ---------------
*/

BigInt BigInt::operator/(const std::string& num) const {
    return *this / BigInt(num);
}


/*
    String / BigInt
    ---------------
*/

BigInt operator/(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) / rhs;
}


/*
    BigInt % String
    ---------------
*/

BigInt BigInt::operator%(const std::string& num) const {
    return *this % BigInt(num);
}


/*
    String % BigInt
    ---------------
*/

BigInt operator%(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) % rhs;
}


/*
    ===========================================================================
    Arithmetic-assignment operators
    ===========================================================================
*/




/*
    BigInt += BigInt
    ----------------
*/

BigInt& BigInt::operator+=(const BigInt& num) {
    *this = *this + num;

    return *this;
}


/*
    BigInt -= BigInt
    ----------------
*/

BigInt& BigInt::operator-=(const BigInt& num) {
    *this = *this - num;

    return *this;
}


/*
    BigInt *= BigInt
    ----------------
*/

BigInt& BigInt::operator*=(const BigInt& num) {
    *this = *this * num;

    return *this;
}


/*
    BigInt /= BigInt
    ----------------
*/

BigInt& BigInt::operator/=(const BigInt& num) {
    *this = *this / num;

    return *this;
}


/*
    BigInt %= BigInt
    ----------------
*/

BigInt& BigInt::operator%=(const BigInt& num) {
    *this = *this % num;

    return *this;
}


/*
    BigInt += Integer
    -----------------
*/

BigInt& BigInt::operator+=(const long long& num) {
    *this = *this + BigInt(num);

    return *this;
}


/*
    BigInt -= Integer
    -----------------
*/

BigInt& BigInt::operator-=(const long long& num) {
    *this = *this - BigInt(num);

    return *this;
}


/*
    BigInt *= Integer
    -----------------
*/

BigInt& BigInt::operator*=(const long long& num) {
    *this = *this * BigInt(num);

    return *this;
}


/*
    BigInt /= Integer
    -----------------
*/

BigInt& BigInt::operator/=(const long long& num) {
    *this = *this / BigInt(num);

    return *this;
}


/*
    BigInt %= Integer
    -----------------
*/

BigInt& BigInt::operator%=(const long long& num) {
    *this = *this % BigInt(num);

    return *this;
}


/*
    BigInt += String
    ----------------
*/

BigInt& BigInt::operator+=(const std::string& num) {
    *this = *this + BigInt(num);

    return *this;
}


/*
    BigInt -= String
    ----------------
*/

BigInt& BigInt::operator-=(const std::string& num) {
    *this = *this - BigInt(num);

    return *this;
}


/*
    BigInt *= String
    ----------------
*/

BigInt& BigInt::operator*=(const std::string& num) {
    *this = *this * BigInt(num);

    return *this;
}


/*
    BigInt /= String
    ----------------
*/

BigInt& BigInt::operator/=(const std::string& num) {
    *this = *this / BigInt(num);

    return *this;
}


/*
    BigInt %= String
    ----------------
*/

BigInt& BigInt::operator%=(const std::string& num) {
    *this = *this % BigInt(num);

    return *this;
}



/*
    ===========================================================================
    Increment and decrement operators
    ===========================================================================
*/


/*
    Pre-increment
    -------------
    ++BigInt
*/

BigInt& BigInt::operator++() {
    *this += 1;

    return *this;
}


/*
    Pre-decrement
    -------------
    --BigInt
*/

BigInt& BigInt::operator--() {
    *this -= 1;

    return *this;
}


/*
    Post-increment
    --------------
    BigInt++
*/

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    *this += 1;

    return temp;
}


/*
    Post-decrement
    --------------
    BigInt--
*/

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    *this -= 1;

    return temp;
}


/*
    ===========================================================================
    I/O stream operators
    ===========================================================================
*/


/*
    BigInt from input stream
    ------------------------
*/

std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string input;
    in >> input;
    num = BigInt(input);  // remove sign from value and set sign, if exists

    return in;
}


/*
    BigInt to output stream
    -----------------------
*/

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    if (num.sign == '-')
        out << num.sign;
    out << num.value;

    return out;
}

