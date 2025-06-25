#include "BigInt.hpp"
#include "./Maths/Miller-Rabin.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

int main(){
    BigInt composite, bigcomposite, prime, bigprime;
    composite = 48;
    prime = 7;
    bigcomposite = "12937129319831923138918";
    bigprime = "721960629061904110756973047896820529650646626913987914193058081946869300323505631503613785147235083747598529092067962512605338801581684234964490805070294572618497933653948906694134011361476095650659862416376029070102910970128990995504864728056436234936733309062195718092793663000312918831684212336353";

    // Testa composite
    cout << composite << " is ";
    if (MillerRabin(composite)) {
        cout << "prime";
    } else {
        cout << "composite";
    }
    cout << endl;

    // Testa prime
    cout << prime << " is ";
    if (MillerRabin(prime)) {
        cout << "prime";
    } else {
        cout << "composite";
    }
    cout << endl;

    // Testa bigcomposite
    cout << bigcomposite << " is ";
    if (MillerRabin(bigcomposite)) {
        cout << "prime";
    } else {
        cout << "composite";
    }
    cout << endl;

    // Testa bigprime
    cout << bigprime << " is ";
    if (MillerRabin(bigprime)) {
        cout << "prime";
    } else {
        cout << "composite";
    }
    cout << endl;

    return 0;
}