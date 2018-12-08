// constructing lock_guard with adopt_lock

//NOTE: needs to be compiled with flag -pthread.

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include "LockGuard.h"    // chal::LockGuard

#define NUM_THDS 30

std::mutex mtx;           // mutex for critical section

/** @brief thread print function
 *
 * Takes in an int and a string
 * Prints to standard out
 * NOT thread-safe: must be called within a thread-safe scope
 */
void thd_printer(int id, std::string msg) {
    std::cout << "thread" << id << ": " << msg << std::endl;
}


void thd_worker (int id) {
    mtx.lock();
    chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);

    std::string msg = "testing!";

    //std::cout << "thread #" << id << '\n';
    thd_printer(id, msg);
}


int main ()
{
    std::thread threads[NUM_THDS]; //creates an array of NUM_THDS of thread objects
    
    // spawn NUM_THDS threads:
    for (int i=0; i<NUM_THDS; ++i)
        threads[i] = std::thread(thd_worker,i+1);

    for (auto& th : threads) th.join();

    return 0;
}
