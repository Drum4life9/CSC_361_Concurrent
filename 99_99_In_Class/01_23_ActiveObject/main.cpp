#include <iostream>
#include <thread>

#include "active_object.h"

using namespace std;

bool check_prime(long n)
{
    bool is_prime = true;
    for (int i = 2; i < n; ++i)
        if (n % i == 0) {
            is_prime = false;
            break;
        }
    return is_prime;

}

int main()
{
    ActiveObject ao;
    ao.start();

    while(true){
        cout << "Enter a number: ";
        long n;
        cin >> n;
        if( n == 0)
            break;
        ao.enqueue([n] {
            bool result = check_prime(n);
            cout << n << " is " << (result ? "prime" : "not prime") << endl;
        });

    }
    ao.stop();
    ao.join();
    cout << "end of program!" << endl;
}
