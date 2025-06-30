#include "modularArithmetics.hpp"
#include <chrono>

namespace BigIntmod{
//returns a+b in mod
BigInt add(const BigInt& a, const BigInt& b, const BigInt& mod){
    return (a+b)%mod;
}

//returns a*b in mod
BigInt mul(const BigInt& a, const BigInt& b, const BigInt& mod){
    return (a*b)%mod;
}

//return a^b in mod
BigInt exp(BigInt a, BigInt b, const BigInt& mod){
    auto start = std::chrono::high_resolution_clock::now();
    a %= mod;
    
    BigInt result =1;
    while(b>0){
        if(b%2==1){
            result = (result*a)%mod;
        }
        b/=2;
        a = (a*a)%mod;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std:: cout << "Result: " << result << std::endl;    
    std::cout << "BigInt exp took " << elapsed.count() << " seconds." << std::endl;
    return result;
}

//return a^(-1) in mod
BigInt inv(const BigInt& a, const BigInt& mod){
    
    auto[g,x,y] = euclideanExtended(a,mod);

    if(g != 1) return -1;   //a and mod are not coprime, there is no inverse

    return x%mod;
}

}
