#include "./Maths/BigInt/BigInt.hpp"
#include <gmpxx.h>
#include "./Maths/BigInt/modularArithmetics.hpp"
#include <chrono>


mpz_class mpz_exp(mpz_class a, mpz_class b, mpz_class m) {
    auto start = std::chrono::high_resolution_clock::now();
    a = a%m;
    mpz_class result = 1;
    while (b > 0) {
        if(b%2==1){
            result = (result*a)%m;
        }
        a = (a*a)%m;
        b /= 2;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Result: " << result << std::endl;
    std::cout << "mpz_exp took " << elapsed.count() << " seconds." << std::endl;
    return result;
}
int main(){
    mpz_class a("12937129319831923138917129371293198319231389171293712931983192313891712937129319831923138917");
    BigInt big_a("12937129319831923138917129371293198319231389171293712931983192313891712937129319831923138917");
    
    mpz_class b("1231874281471823714914248337123187428147182371491424833712318742814718237149142483371231874281471823714914248337");
    BigInt big_b("1231874281471823714914248337123187428147182371491424833712318742814718237149142483371231874281471823714914248337");
    
    mpz_class m("3123817318481783172834312381731848178317283431238173184817831728343123817318481783172834");
    BigInt big_m("3123817318481783172834312381731848178317283431238173184817831728343123817318481783172834");

    mpz_exp(a, b, m);
    mod::exp(big_a, big_b, big_m);
}