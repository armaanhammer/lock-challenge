// constructing lock_guard with adopt_lock

//NOTE: needs to be compiled with flag -pthread.

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include <chrono>         // std::chrono
#include <condition_variable>
#include <random>
#include <vector>

#include "LockGuard.h"    // chal::LockGuard


#define NUM_THDS 3


/// Globals
std::mutex mtx;                 //shared mutex
std::condition_variable cond;   //shared condition variable
bool DEBUG = false;



/** \brief thread print function
 *
 *  \param id an integer that defines thread number
 *  \param msg a string containing message to be printed
 *
 * Prints to standard out
 * NOT thread-safe; must be called within a thread-safe scope
 */
void thd_printer(int id, std::string msg) {
    //bool DEBUG = true;

    if(DEBUG) std::cout << "made it to thd_printer" << std::endl;

    //mtx.lock();
    if(DEBUG) std::cout << "made it to mtx.lock" << std::endl;
    
    //chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);
    if(DEBUG) std::cout << "made it to LockGuard" << std::endl;
    
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
void thd_worker (const int this_id, int &next_thd) {
    bool DEBUG = true;

    if(DEBUG) std::cout << "this: " << this_id 
                        << " next: " << next_thd << std::endl;

    /* PSUEDO CODE
     * block on condition_variable and if this_id != &next_thd
     * sleep for 1-5 secs
     * lock mutex
     * lock_guard(mutex)
     * increment next_thd
     *   if this is topmost thread, reset next_thd
     *   else increment next_thd
     * call thread_printer function, passing in mutex
     *   in thread printer function:
     *     capture mutex from calling function
     *     print
     *
     */ 
 
    thd_printer(this_id, "starting, waiting.");

    ///< sleep to simulate work
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int j=0; j<5 ; j++) {
    
        /// lock mutex
        mtx.lock();
        std::unique_lock<std::mutex> lck (mtx, std::adopt_lock);

        /// if topmost thread, reset next_thd
        if(next_thd == NUM_THDS) next_thd = 0;
        else( ++next_thd);
        
        if(DEBUG) std::cout << "this: " << this_id 
                            << " next: " << next_thd << std::endl;   
    }
    

    std::string msg = "testing!";

    thd_printer(this_id, "testing!");
    
    if(DEBUG) std::cout << "this: " << this_id 
                        << " returned from thd_printer" << std::endl;
}


int main () {
    //bool DEBUG = true;

    //std::vector<std::thread> threads
    std::thread threads[NUM_THDS]; //creates an array of NUM_THDS of thread objects
    std::mutex mutexes[NUM_THDS]; //creates an array of NUM_THDS of mutex objects

    int next_thd = 0; 
    
    /// spawn NUM_THDS threads:
    for (int i=0; i<NUM_THDS; ++i) {
        
        if (i == NUM_THDS-1) next_thd=0;
        else next_thd = i+1;

        if(DEBUG) std::cout << "main for loop #" << i << std::endl;

        threads[i] = std::thread(thd_worker, i, std::ref(next_thd));
    }

    // wait for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cond.notify_all()  ///< start sequence


    if(DEBUG) std::cout << "main join for starting" << std::endl;
    for (auto& th : threads) {
        th.join();
        
        if(DEBUG) std::cout << "an instance of main for join loop" << std::endl;
    }


    return 0;
}
