#include <queue>
#include <thread>
#include <functional>
#include <stdexcept>
#include <iostream>

#include "active_object.h"
#include "scheduler.h"
#include "future.h"
#include "qentry_base.h"

using std::function;
using std::unique_lock;
using std::mutex;
using std::deque;
using std::thread;
using std::unique_lock;

// default construct everything
ActiveObject::ActiveObject(Scheduler* s, int id)
    : scheduler{s}, id{id}
{}

// assume the thread has been killed already
ActiveObject::~ActiveObject() 
{}

void ActiveObject::start() 
{
    th = thread([this]() {
        this->worker();
    });
}

void ActiveObject::shut_down() 
{
    this->enqueue<void>([this]() { this->working = false;});
}

void ActiveObject::stop() 
{
    working = false;
    cv.notify_all();
}

void ActiveObject::join() 
{
    if (th.joinable())
        th.join();
}

Scheduler* ActiveObject::get_scheduler() const {
    return scheduler;
}

std::thread::id ActiveObject::get_thread_id() const 
{
    return th.get_id();
}

void ActiveObject::worker() {
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

            qeb = work_queue.back();
            work_queue.pop_back();
        }
        qeb->run_and_complete();
        delete qeb;

        if (this->get_num_tasks() == 0) {
            this->scheduler->steal_work(this);
        }
    }

}

void ActiveObject::give_my_function_to_another(ActiveObject* obj2) {
    //lock queue mutex
    std::unique_lock lck {queue_mutex};
    //get function from the front (this is the FIFO side)
    auto func = this->work_queue.front();
    work_queue.pop_front();
    cv.notify_one();
    obj2->work_queue.push_back(func);
}