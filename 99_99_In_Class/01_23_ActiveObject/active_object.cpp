#include <queue>
#include <thread>
#include <functional>

#include "active_object.h"

using std::function;
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

void ActiveObject::start(){
    th = thread([this](){worker();});
}

void ActiveObject::shut_down() {
    this->enqueue([this]{this->working = false;});
}

void ActiveObject::stop(){
    working = false;
    cv.notify_one();
}

void ActiveObject::join(){
    if(th.joinable()){
        th.join();
    }
}

bool ActiveObject::check_is_waiting() const {
    return working;
}


void ActiveObject::worker(){
    working = true;

    while(working){
        func_type func;

        {
            unique_lock lck{queue_mutex};
            while(working && work_queue.empty()){
                cv.wait(lck);
            }

            if(!working)
                break;

            func = std::move(work_queue.front());
            work_queue.pop();
        }

        func();
    }

}