#include "modularArithmetics.hpp"

namespace mod{
//returns a+b in mod
mpz_class add(const mpz_class& a, const mpz_class& b, const mpz_class& mod){
    return (a+b)%mod;
}

//returns a*b in mod
mpz_class mul(const mpz_class& a, const mpz_class& b, const mpz_class& mod){
    return (a*b)%mod;
}

//return a^b in mod
mpz_class exp(mpz_class a, mpz_class b, const mpz_class& mod){
    a %= mod;
    
    mpz_class result =1;
    while(b>0){
        if(b%2==1){
            result = (result*a)%mod;
        }
        b/=2;
        a = (a*a)%mod;
    }
    return result;
}

//return a^(-1) in mod
mpz_class inv(const mpz_class& a, const mpz_class& mod){
    
    auto[g,x,y] = euclideanExtended(a,mod);

    if(g != 1) return -1;   //a and mod are not coprime, there is no inverse

    return (x % mod + mod) % mod; // ensures the result is non-negative

}

}
