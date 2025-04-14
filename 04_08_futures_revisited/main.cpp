#include <iostream>
#include <thread>
#include <string>


#include "active_object.h"
#include "future.h"

using namespace std;

bool check_prime(long n)
{
    bool is_prime = true;
    for (int i = 2; i < n; ++i)
        if (n % i == 0) {
            is_prime = false;
            break;
        }

    if (is_prime)
        cout << n << " is prime" << endl;
    else
        cout << n << " is not prime" << endl;
    this_thread::sleep_for(10s);

    return is_prime;
}

void test1() {
    Future<int> f1;
    Future<string> f2;

    auto th1 = thread([p = f1.get_promise_ptr()]() {
        cout << "th1 working" << endl;
        this_thread::sleep_for(2s);
        p->complete(420);
        this_thread::sleep_for(2s);
        cout << "th1 ending" << endl;
    });

    auto th2 = thread([p = f2.get_promise_ptr()]() {
        cout << "th2 working" << endl;
        this_thread::sleep_for(5s);
        p->complete("Hello, World!");
        this_thread::sleep_for(5s);
        cout << "th2 ending" << endl;
    });

    cout << "main is working" << endl;
    cout << f1.get() << endl;
    cout << f2.get() << endl;
    th1.join();
    th2.join();
}

void test2()
{
    ActiveObject ao;
    ao.start();
    vector<Future<bool>> futs;

    while (true) {
        cout << "Enter a number: ";
        long n;
        cin >> n;
        if (n == 0)
            break;
        auto fut = ao.enqueue<bool>([n]() -> bool { return check_prime(n); });
        futs.push_back(fut);
    }

    for (auto f : futs) {
        if (f.get())
            cout << "main said prime" << endl;
        else
            cout << "main said NOT prime" << endl;
    }

    ao.stop();
    ao.join();
    cout << "bye" << endl;
}

int main() {
    test1();
    //test2();

    return 0;
}