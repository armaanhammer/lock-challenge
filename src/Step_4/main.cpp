/// 
/// \file main.cpp
/// \brief contains all functions to demo thread synch
///
/// \note set DEBUG to true in globals to turn on debug output file-wide, or 
///       uncomment it inside a function to enable function-specific debug output.
/// 
/// \note needs to be compiled with flag -pthread.
///
/// \author Armaan Roshani
/// 
///



#include <iostream>             // std::cout
#include <thread>               // std::thread
#include <mutex>                // std::mutex, std::adopt_lock, std::unique_lock
#include <chrono>               // std::chrono
#include <condition_variable>   // std::condition_variable
#include <random>               // std::default_random_generator

#include "LockGuard.h"          // chal::LockGuard

#define NUM_THDS 3              // number of threads to execute


// Globals
std::mutex mtx;                 // shared mutex to signal threads
std::mutex prnt_mtx;            // shared mutex to print from threads
std::condition_variable cond;   // shared condition variable
bool DEBUG = false;             // turn on debug messages



/// 
/// \brief DEBUG PRINTER function
///
/// \param id an integer that defines thread number (0 for main)
/// \param msg a string containing message to be printed
///
/// Prints to standard out
/// \warning NOT thread-safe; must be called within a thread-safe scope
///
void debugPrinter(int id, std::string msg) {
    
    std::cerr << "***\tDEBUG from" << id+1 << ": " 
              << msg << "\t***" << std::endl;
}

/// 
/// \brief DEBUG PRINTER function overload
///
/// \overload debugPrinter
///
/// \param id an integer that defines thread number (0 for main)
/// \param next_thd an integer indicating next thread to run
///
void debugPrinter(int id, int next_thd) {
    
    std::cerr << "***\tDEBUG from" << id+1 << ": " 
              << " next: " << next_thd << "\t***" << std::endl;
}

/// 
/// \brief DEBUG PRINTER function overload
///
/// \overload debugPrinter
///
/// \param id an integer that defines thread number (0 for main)
/// \param msg a string containing message to be printed
/// \param next_thd an integer indicating next thread to run
///
void debugPrinter(int id, std::string msg, int next_thd) {
    
    std::cerr << "***\tDEBUG from" << id+1 << ": " 
              << msg << " next: " << next_thd << "\t***" << std::endl;
}



/// 
/// \brief thread print function
///
/// \param id an integer that defines thread number
/// \param msg a string containing message to be printed
///
/// Prints to standard out
/// \warning NOT thread-safe; must be called within a thread-safe scope
///
void thd_printer(int id, std::string msg) {
    //bool DEBUG = true;

    if(DEBUG) 
        debugPrinter(id, "made it to thd_printer");

    prnt_mtx.lock();
    if(DEBUG) 
        debugPrinter(id, "made it to mtx.lock");
    
    chal::LockGuard<std::mutex> lck (prnt_mtx, std::adopt_lock);
    if(DEBUG) 
        debugPrinter(id, "made it to LockGuard");
    
    std::cout << "thread" << id+1 << ": " << msg << std::endl;
}


/// 
/// \brief thread worker function
///
/// \param id an integer passed by val defining current thread number
/// \param next_thd an integer passed by ref that keeps track of thread order
/// \param rand_e a pseudo-random number generator engine passed by ref
///
/// Upon startup, function blocks on condition_signal. Upon receipt of condition_signal,
/// function sleeps at random for between 1 and 5 seconds, then signals next thread and 
/// goes back to blocking on condition_signal.
///
void thd_worker (const int id, int &next_thd, std::default_random_engine &rand_e) {
    //bool DEBUG = true;
    
    if(DEBUG) 
        debugPrinter(id, next_thd);

    int wait_tm; // time to randomize

    thd_printer(id, "starting, waiting.");

    // repeat forever
    while(1) {
    
        // lock mutex
        if(DEBUG) 
            debugPrinter(id, "ABOUT TO LOCK", next_thd);
        mtx.lock();
        std::unique_lock<std::mutex> locker (mtx, std::adopt_lock);
        
        // wait for condition signal
        // Upon condition signal, check if current thread is next
        // if yes continue, if not keep waiting
        // lambda function creates condition predicate 
        if(DEBUG) 
            debugPrinter(id, "made it to cond.wait");   
        cond.wait(locker, [&]() { return id == next_thd; });
        if(DEBUG) 
            debugPrinter(id, "made it past cond.wait");   

        thd_printer(id, "signal received, doing work ....");

        // generate pseudo-random value between 1 and 5 seconds
        wait_tm = 1 + rand_e() % 5; 
         
        // sleep to simulate work
        if(DEBUG) 
            debugPrinter(id, "sleeping for:", wait_tm);
        std::this_thread::sleep_for(std::chrono::seconds(wait_tm));

        thd_printer(id, "done with work, signal next thread");
        if(DEBUG) 
            debugPrinter(id, "returned from thd_printer");

        // if topmost thread, reset next_thd
        if(next_thd == NUM_THDS-1) { 
            next_thd = 0;
        }
        // otherwise, just increment
        else {
            ++next_thd;
        } 

        if(DEBUG) 
            debugPrinter(id, "ABOUT TO NOTIFY", next_thd);
        cond.notify_all();  // restart sequence
    }
}



int main () {
    //bool DEBUG = true;
    int id = -1; // identifier to pass to Debug Printer

    std::thread threads[NUM_THDS]; /// create an array of NUM_THDS thread objects
    int next_thd = -1; // prevent a spurious wake from prematurely activating thread 0

    std::default_random_engine rand_e; /// create a pseudo-random engine
   
    std::cout << "main: starting all threads" << std::endl;

    // spawn NUM_THDS threads:
    for (int i=0; i<NUM_THDS; ++i) {
        
        if(DEBUG) 
            debugPrinter(id, "main for loop #");

        // populate the array of thread objects
        // pass in: * their unique ID by value
        //          * an integer to keep track of thread order by reference
        //          * a shared psuedo-random number generator by reference
        threads[i] = std::thread(thd_worker, i, std::ref(next_thd), std::ref(rand_e));
    }

    /// wait for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if(DEBUG) 
        debugPrinter(id, "waited three seconds");

    cond.notify_all();  // start sequence
    next_thd = 0;       // allow thread 0 to be activated

    if(DEBUG) 
        debugPrinter(id, "main join for starting");

    // clean up
    for ( auto& th : threads ) {
        th.join();
        
        if(DEBUG) {
            debugPrinter(id, "an instance of main for join loop");
        }
    }

    return 0;
}
