// constructing lock_guard with adopt_lock
// NOTE: needs to be compiled with flag -pthread.
// SOURCE: http://www.cplusplus.com/reference/mutex/lock_guard/

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include "LockGuard.h"    // chal::LockGuard

#define THREAD_NUM 3

std::mutex mtx;           // mutex for critical section

void print_thread_id (int id) {
    mtx.lock();
    //std::lock_guard<std::mutex> lck (mtx, std::adopt_lock);
    chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);

    std::cout << "thread #" << id << '\n';
}

int main ()
{
    std::thread threads[THREAD_NUM]; //creates an array of THREAD_NUM number of thread objects
    // spawn THREAD_NUM threads:
    for (int i=0; i<THREAD_NUM; ++i)
        threads[i] = std::thread(print_thread_id,i+1);

    for (auto& th : threads) th.join();

    return 0;
}
