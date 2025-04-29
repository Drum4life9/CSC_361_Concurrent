#include <memory>

#include "scheduler.h"
#include "active_object.h"

Scheduler::Scheduler(int num, bool is_stealer) : current(0), is_stealer(is_stealer)
{
    for (int i = 0; i < num; ++i) {
        workers.push_back(std::make_unique<ActiveObject>(this, i));
        workers.back()->start();
    }
}

void Scheduler::terminate() 
{
    for (auto& w : workers) {
        w->stop();
        w->join();
    }
}

bool Scheduler::is_worker(std::thread::id tid) const 
{
    for (auto& w : workers) {
        if (tid == w->get_thread_id())
            return true;
    }
    return false;
}

void Scheduler::steal_work(ActiveObject* obj) const {
    if (!is_stealer) {
        return;
    }
    int most_work_index = -1;
    int most_work_num = 0;
    for (int i = 0; i < workers.size(); ++i) {
        //check stuff here
        int cur_work_num = workers[i]->get_num_tasks();
        if (cur_work_num > most_work_num && workers[i]->get_my_id() != obj->get_my_id()) {
            most_work_index = i;
            most_work_num = cur_work_num;
        }
    }
    if (most_work_index > -1) {
        ActiveObject* obj_to_steal_from = workers[most_work_index].get();
        obj_to_steal_from->give_my_function_to_another(obj);
        std::cout << "ID " << obj->get_my_id() << " stole some work from AO " << obj_to_steal_from->get_my_id() << std::endl;
    }


}