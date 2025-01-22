#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <condition_variable>

using namespace std;

double compute_arctan(const double& x, mutex& m) {
    //comment out this next line and the ending for mutexes
    m.lock();
    //
    double ret = atan(sqrt(exp(x))) + 10;
    cout << "Arctan function: " << ret << endl;
    //
    m.unlock();
    //
    return ret;
}

double compute_arctan_2(const double& x, mutex& m) {
    //
    m.lock();
    //
    double ret =  atan(sqrt(x / 3)) + 5;
    cout << "Arctan 2 function: " << ret << endl;
    //
    m.unlock();
    //
    return ret;
}

int main() {
    // Problem 1
    //crazy that mutexes fixed the issues.....
    double x = 1;
    mutex m;
    jthread t1(compute_arctan, std::ref(x), std::ref(m));
    jthread t2(compute_arctan_2, std::ref(x), std::ref(m));


    // Problem 2
    condition_variable cv;

    // Now we could create two functions, one with cv.wait(), which waits for
    //      work to be done, and one with cv.notify_one(), which will be called
    //      after some work is "done", then the cv.wait() will open up and allow
    //      more work to be done in the thread that had to wait for the producer.
    //      Seems like this would work well with an event driven architecture!


    // Problem 3
    /*
     * The reason we can't just use if(!ready) is that this block of code will
     *      only get executed once. The thread would run through the if statement,
     *      if it is not ready then the thread will bypass the waiting step, and
     *      move on executing the code below the if statement, which will likely
     *      cause some pretty catastrophic results since the shared resource is
     *      not yet ready to accept another thread working on it. By using a
     *      while loop, this section (cv.wait()) will continually get called
     *      until the first thread has finished up and changed the global ready
     *      boolean to true, then the waiting thread can stop waiting and use
     *      the shared resource that it requires to continue doing its job.
     */
}
