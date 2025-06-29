#include "GCD.hpp"

using namespace std;

//returns (g,x,y) such that a*x + b*y = g;
tuple<BigInt,BigInt,BigInt> euclideanExtended(BigInt a, BigInt b){
        BigInt x0 = 1, y0 = 0;
        BigInt x1 = 0, y1 = 1;

        while(b!=0){
            BigInt q = a/b;
            BigInt r = a%b;

            a = b; b =r;

            BigInt tempx, tempy;

            tempx = x0 - q * x1;
            tempy = y0 - q * y1;
        
            x0 = x1; y0 = y1;
            x1 = tempx; y1 = tempy;
        }
        return {a,x0,y0};
}