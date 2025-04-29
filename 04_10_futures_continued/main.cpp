#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <cmath>
#include <chrono>

#include "scheduler.h"
#include "active_object.h"
#include "future.h"

using namespace std;

double dotp(vector<double>& a, vector<double>& b, int s, int e) {
    double sum = 0.0;
    for (int i = s; i < e; ++i) {
        for (int j = 0; j < 50; ++j)
            sum += sqrt(abs(a[i])) * sqrt(abs(b[i]));
    }
    return sum;
}

double dotpfj(Scheduler& sch, vector<double>& a, vector<double>& b, 
                int s, int e) 
{
    if (e - s < 10'000) {
        return dotp(a, b, s, e);
    }

    int mid = s + (e - s) / 2;
    auto f = sch.schedule<double>([&sch, &a, &b, s, mid]() { 
        return dotpfj(sch, a, b, s, mid); 
    });
    double r = dotpfj(sch, a, b, mid, e);
    double l = f.get();
    return r + l;
}

void test_scheduler2() {
    Scheduler s(6, true);
    vector<double> a(2'000'000);
    vector<double> b(2'000'000);
    for (int i = 0; i < a.size(); ++i) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    int start = 0;
    int end = a.size();

    cout << "starting parallel\n";
    auto beg = chrono::system_clock::now();
    auto f = s.schedule<double>([&s, &a, &b, start, end]() {
        return dotpfj(s, a, b, start, end);
    });

    double val = f.get();
    auto fin = chrono::system_clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(fin-beg).count();

    cout << val << " took " << dur << " ms." << endl;

    cout << "starting serial\n";
    beg = chrono::system_clock::now();
    val = dotp(a, b, 0, a.size());
    fin = chrono::system_clock::now();
    dur = chrono::duration_cast<chrono::milliseconds>(fin-beg).count();
    cout << val << " took " << dur << " ms." << endl;
    s.terminate();
}

void test4_Stealer() {
    Scheduler s(12, true);

    for (int i = 0; i < 100; ++i) {
        s.schedule<void>([i]() { 
            chrono::seconds dur(5 - i/20);
            this_thread::sleep_for(dur);
            // cout << this_thread::get_id() << " hi " << i << endl;
            });
    }

    s.terminate();
}

void test5_Not_Stealer() {
    Scheduler s(12, false);

    for (int i = 0; i < 100; ++i) {
        s.schedule<void>([i]() {
            chrono::seconds dur(5 - i/20);
            this_thread::sleep_for(dur);
            // cout << this_thread::get_id() << " hi " << i << endl;
        });
    }

    s.terminate();
}

int main() {
    cout << "starting stealer\n";
    auto beg = chrono::system_clock::now();
    test4_Stealer();
    auto fin = chrono::system_clock::now();
    auto dur = chrono::duration_cast<chrono::milliseconds>(fin-beg).count();
    cout << "The stealer took " << dur << " ms." << endl;

    cout << "starting Not stealer\n";
    beg = chrono::system_clock::now();
    test5_Not_Stealer();
    fin = chrono::system_clock::now();
    dur = chrono::duration_cast<chrono::milliseconds>(fin-beg).count();
    cout << "The not_stealer took " << dur << " ms." << endl;


}