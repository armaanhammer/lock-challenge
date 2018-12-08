// constructing lock_guard with adopt_lock

//NOTE: needs to be compiled with flag -pthread.

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include <random>

#include "LockGuard.h"    // chal::LockGuard
#include <vector>

#define NUM_THDS 30


/// Globals
std::mutex mtx;           // mutex for critical section



/** \brief thread print function
 *
 *  \param id an integer that defines thread number
 *  \param msg a string containing message to be printed
 *
 * Prints to standard out
 * NOT thread-safe; must be called within a thread-safe scope
 */
void thd_printer(int id, std::string msg) {
    std::cout << "thread" << id << ": " << msg << std::endl;
}


/** \brief thread worker function
 *
 *  \param this_id an integer that defines current thread number
 *  \param next_id an integer that defines next thread number
 *
 *  Upon startup, function blocks on condition_signal. Upon receipt of condition_signal,
 *  function sleeps at random for between 1 and 5 seconds, then signals next thread and 
 *  goes back to blocking on condition_signal.
 */
void thd_worker (int this_id, int next_id) {
    mtx.lock();
    chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);

    // sleep
    std::this_thread::sleep_for(chrono::second(2));

    // debug
    std::cout << "this: " << this_id 
              << " next: " << next_id << std::endl;


    std::string msg = "testing!";

    thd_printer(this_id, msg);
}


int main ()
{
    //std::vector<std::thread> threads
    std::thread threads[NUM_THDS]; //creates an array of NUM_THDS of thread objects
    
/*
    for (int th : sizeof(threads))
        threads[th] = std::thread(thd_worker,th+1,th+2);
*/
    int this_id, next_id;

    /// spawn NUM_THDS threads:
    for (int i=0; i<NUM_THDS; ++i) {
        this_id = i+1;
        if (i == NUM_THDS-1) next_id=1;
        else next_id = i+2;
        threads[i] = std::thread(thd_worker, this_id, next_id);
    }

    std::cout << "uh?" << std::endl;

    for (auto& th : threads) th.join();

    return 0;
}
