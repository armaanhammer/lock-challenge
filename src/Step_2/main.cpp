// constructing lock_guard with adopt_lock

//NOTE: needs to be compiled with flag -pthread. Not yet sure why.

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include "LockGuard.h"    // chal::LockGuard

std::mutex mtx;           // mutex for critical section

void print_thread_id (int id) {
    mtx.lock();
    //std::lock_guard<std::mutex> lck (mtx, std::adopt_lock);
    chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);

    std::cout << "thread #" << id << '\n';
}

int main ()
{
    std::thread threads[10]; //creates an array of 10 thread objects
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_thread_id,i+1);

    for (auto& th : threads) th.join();

    return 0;
}
