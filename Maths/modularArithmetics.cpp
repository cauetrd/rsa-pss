#include "../BigInt.hpp"
#include "EuclideanExtended.hpp"

using namespace std;


//returns a+b in mod
BigInt add(BigInt a, BigInt b, BigInt mod){
    return (a+b)%mod;
}

//returns a*b in mod
BigInt mul(BigInt a, BigInt b, BigInt mod){
    return (a*b)%mod;
}

//return a^b in mod
BigInt exp(BigInt a, BigInt b, BigInt mod){
    a %= mod;
    
    BigInt result =0;
    while(b>0){
        if(b%2==1){
            result = (result+a)%mod;
        }
        b/=2;
        a = (a*a)%mod;
    }
}

//return a^(-1) in mod
BigInt inv(BigInt a, BigInt mod){
    
    auto[g,x,y] = euclideanExtended(a,mod);

    if(g != 1) return -1;   //a and mod are not coprime, there is no inverse

    return x%mod;
}
