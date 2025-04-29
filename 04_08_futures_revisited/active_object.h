#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "future.h"

class ActiveObject {
    template <typename R>
    using func_type = std::function<R()>;

    class QEBase {
    public:
        virtual ~QEBase() {}

        virtual void run_and_complete() = 0;
    };

    template <typename R>
    class QueueEntry : public QEBase {
        using func_type = std::function<R()>;

    public:
        QueueEntry(const func_type& ft, const std::shared_ptr<Promise<R>>& prom)
            : task(ft), promise(prom)
        {}

        QueueEntry(const func_type& ft)
            : task(ft), promise{}
        {}

        virtual void run_and_complete() {
            if (promise)
                promise->complete(task());
            else
                task();
        }

    private:
        func_type task;
        std::shared_ptr<Promise<R>> promise;
    };


public:
    // Create the active object, initialize the queue, start
    // the thread running.
    ActiveObject();

    // tell the worker thread to shut down, and join that thread
    ~ActiveObject();

    void start();

    // push a task (a function) onto our queue. This is a tricky
    // little function! It uses some very modern C++ techniques.
    template <typename R>
    Future<R> enqueue(func_type<R> func) {
        std::unique_lock<std::mutex> lck { queue_mutex };
        auto fut = Future<R>();
        work_queue.push(new QueueEntry<R>(func, fut.get_promise_ptr()));
        cv.notify_one();
        return fut;
    }

    // put a poison pill into the queue so that it shuts down.
    // void shut_down();

    void stop();

    void join();

private:
    // the function that is running on the thread we "own".
    void worker();

    std::queue<QEBase*> work_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool working;
    std::thread th;
};

#endif
