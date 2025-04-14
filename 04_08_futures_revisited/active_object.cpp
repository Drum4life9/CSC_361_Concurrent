#include <queue>
#include <thread>
#include <functional>
#include <stdexcept>

#include "active_object.h"
#include "future.h"

using std::function;
using std::unique_lock;
using std::mutex;
using std::queue;
using std::thread;
using std::unique_lock;

// default construct everything
ActiveObject::ActiveObject()
{}

// assume the thread has been killed already
ActiveObject::~ActiveObject()
{
}

void ActiveObject::start() {
    th = thread([this]() {
        this->worker();
    });
}

// Interesting interface problem -- how to handle void tasks
// void ActiveObject::shut_down()
// {
//     this->enqueue([this]() { this->working = false;});
// }

void ActiveObject::stop() {
    working = false;
    cv.notify_one();
}

void ActiveObject::join() {
    if (th.joinable())
        th.join();
}

void ActiveObject::worker()
{
    working = true;

    while (working) {
        QEBase* qeb;
        {
            unique_lock<mutex> lck {queue_mutex};
            while (working && work_queue.empty()) {
                cv.wait(lck);
            }

            if (!working)
                break;

            qeb = work_queue.front();
            work_queue.pop();
        }
        qeb->run_and_complete();
        delete qeb;
    }
}