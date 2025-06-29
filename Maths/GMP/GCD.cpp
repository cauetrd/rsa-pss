#include "GCD.hpp"

using namespace std;

//returns (g,x,y) such that a*x + b*y = g;
tuple<mpz_class,mpz_class,mpz_class> euclideanExtended(mpz_class a, mpz_class b){
        mpz_class x0 = 1, y0 = 0;
        mpz_class x1 = 0, y1 = 1;

        while(b!=0){
            mpz_class q = a/b;
            mpz_class r = a%b;

            a = b; b =r;

            mpz_class tempx, tempy;

            tempx = x0 - q * x1;
            tempy = y0 - q * y1;
        
            x0 = x1; y0 = y1;
            x1 = tempx; y1 = tempy;
        }
        return {a,x0,y0};
}

mpz_class gcd(mpz_class a, mpz_class b) {
    while(b>0){
        a%=b;
        mpz_class aux = a;
        a = b;
        b = aux;
    }
    return a;
}