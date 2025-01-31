#ifndef ACTIVEOBJECT_ACTIVE_OBJECT_H
#define ACTIVEOBJECT_ACTIVE_OBJECT_H

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

class ActiveObject {
    using func_type = std::function<void()>;
public:
    // Create the active object, initialize the queue, start the thread running.
    ActiveObject();

    // tell the worker thread to shut down, and join that thread
    ~ActiveObject();

    void start();

    // push a task (a function) onto our queue.
    void enqueue(auto&& func) {
        {
            std::unique_lock lck{queue_mutex};
            work_queue.push(std::move(func));
        }
        cv.notify_one();
    }

    // put a poison pill into the queue so that it shuts down.
    void shut_down();

    void stop();

    void join();

    bool check_is_waiting() const;

private:
    // the function that is running on the thread we "own".
    void worker();

    std::queue<func_type> work_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool working;
    std::thread th;
};

#endif //ACTIVEOBJECT_ACTIVE_OBJECT_H
