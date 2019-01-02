# Step 3 Appendix

## Initial Prompt

> 3) Use c++11 `std::condition_var` in solution from #3 to synchronize threads
> ```     
> -- Psuedo Code --
>
>      main:
>
>        1) start: thread 1, thread 2, thread 3
>        2) all threads block on condition_signal
>        3) main signals condition_signal
>        4) join all threads to prevent exit main // gimmee
>
>        thread 1:
>           break from condition_signal due to step 3
>           sleep (random(1,5))
>           signal thread 2
>           wait on condition_signal
>
>       example output:
>           main: starting all threads
>           thread1: starting, waiting.
>           thread2: starting, waiting.
>           thread3: starting, waiting.
>           main: starting thread 1.
>           thread1: signal received, doing work ....
>           thread1: done with work, signal next thread
>           thread2: signal received, doing work ....
>           thread2: done with work, signal next thread
>
>           on and on and on until you CTRL-C the program
> ```

## Parsing Prompt:

Uncertain what "solution from #3" in `Use c++11 std::condition_var in solution from #3` refers to. Going to assume it is a typo from a previous version of the same prompt, and instead focus on the std function.

Also uncertain what line with //gimmie is talking about. My assumption based on the example output is that all three threads need to loop sequentially. I am uncertain from the prompt whether they should be started and joined repeatedly in main, or if they should run forever by themselves. To attempt to parse this, will write code for a bit and see which is more reasonable.


<br>

Research
---

### Sizing

Want variable sizing (at compile time) - add more or less threads at will and have program perform the same. Feel this is good coding practice, and can potentially reduce complexity.

### How to guarantee ordered thread execution

Thinking of two ways to guarantee ordered thread execution:
* One mutex per thread-to-thread handoff
  * Each possible transition (eg. 1-2 , 2-3 , 3-1) has its own mutex that the 'receiving' thread blocks on.
  * main() is responsible for assigning mutexes to correct threads (eg. thread 3 must be assigned mutex 1 as mutex to unlock)
* One mutex total, with an int passed by reference to maintain count of which thread is next. 
  * each thread must have logic to increment in (eg. thread 3 must 'increment' int to 1)
  
Decided on single mutex and shared int by reference. Reasoning being: each thread already needs to know its ID to print it out. Might as well capitalize on that.

### Storage of thread and objects

* Chosing to continue storing thread objects either in arrays or a vector for ease of joining later.
  * Ran into question about whether to use array vs. vector. 
    * Pros for array:
      * easy
      * quick
    * Cons for array:
      * harder to count size of
      * less expandible
    * Pros for vector:
      * far more expandable
      * possible to easily size
    * Cons for vector:
      * needless complexity
      * slower
      
Decided on array. May update later.

### Indexing

Decided on 0-indexing because easier to code. Will +1 for cout statements so appears as 1-indexing to comply with display output requirements.


### Doxygen Creation

Now that this is becoming slightly more complex, I will start commenting my code with Doxygen styling to facilitate generating that when I am done.

I am generating the mainpages for each step directly from the assicated design_log markdown pages. It is advantagous for the markdown pages to use dynamic links so they are full-path-agnostic, but dynamic links break when Doxygen builds using them. To get around this, once I have a final copy of a step's markdown page, I copy it into the [static_links directory](static_links). From there I manually change all the links to static links into my github repo, and build Doxygen from that. This problem was referenced in Issue #5.

The documentation I am creating for this repo differs slightly from what a normal codebase might have because I include main.cpp and main_no_debug.cpp in my projects here. I want both files to be included in the generated documenation for easy browsing, but I only want main.cpp to be referenced for things like class definitions or *todo* or *bug* enumeration. To accomplish this, I build Doxygen once with all files, then remove the main_no_debug.cpp and build Doxygen again using a different target directory. Then, I manuall overwrite any relevant files in the first directory with files from the second, and push the modified folder. This has so far only been relevant for Step 5, and is mentioned in Issue #1.



<br>

Coding
---

### Threading

Initial idea:

    /* PSUEDO CODE
     * block on condition_variable and if id != &next_thd
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
     
### chal::LockGuard vs. std::unique_lock

Realized that `std::condition_variable` **must** use `std::unique_lock` to wake on notify. This constrains my code, preventing me from using `chal::LockGuard` unless I make significant modificantions. Reading ahead to [Step 4](Step_4.md), I see that I can use it there to protect a resource. Not going to use it for this step.

## Thread Print Function

Prior to disovering I must use `std::unique_lock`, I thought of several ways I could try to use `chal::LockGuard`. Since a mutex created by LockGuard can only be released by leaving scope, and since the main threads needed to keep running, I figured I would create a dedicated function that could be entered and left again to release the mutex. I realized later than I could simply enter and exit a scope by inserting brackets { } anywhere, but I had already created this function, and using it is quite convient. 

```C++
/** \brief thread print function
 *
 *  \param id an integer that defines thread number
 *  \param msg a string containing message to be printed
 *
 * Prints to standard out
 * \warning NOT thread-safe; must be called within a thread-safe scope
 */
void thd_printer(int id, std::string msg) {

    std::cout << "thread" << id+1 << ": " << msg << std::endl;
}
```

## Thread Worker Function

This is the meat of the program.

The thread worker receives a random engine by reference for two reasons:
1. to minimize the memory size.
1. to prevent each thread from duplicating the same behavior. If multiple random engines are copied by value from the same source random engine, they will all share the same seed, and will behave identically. In this case, since all three threads are sharing the same random engine, every call the engine advances all the threads through the same pool, introducing the same randomness that expecte by multiple calls to a random engine from one thread.
   
Inside of the loop, the mutex is first locked then adopted by `std::unique_lock`. As soon as `cond.wait` is called, the mutex is released, and the thread blocks on `std::condition_variable cond`. The thread can wake up for two reasons:
1. because it has received a signal from `std::condition_variable cond`.
1. randomly, a condition called *spurious wake*.

There are multiple threads waiting for the same signal, so in either of those conditions, the thread should not progress until it is supposed to. The multiple threads keep track of whose turn it is next by updating the variable `next_thd` passed in by reference, and using it as a predicate to move past `cond.wait`.

Once the thread has received the signal and confirmed that it is its turn, it invokes the [Thread Print Function](#thread-print-function) then sleeps for a random time determined by the random engine to simulate work. Once the sleep time has completed, it invokes the function again, then updates `next_thd`, and finally notifies the other threads using `const.notify_all`. It is important that it updates the variable prior to notifying to prevent a race condition where it itself could accidently run again.

```C++
/** \brief thread worker function
 *
 *  \param id an integer passed by val defining current thread number
 *  \param next_thd an integer passed by ref that keeps track of thread order
 *  \param rand_e a pseudo-random number generator engine passed by ref
 *
 *  Upon startup, function blocks on condition_signal. Upon receipt of condition_signal,
 *  function sleeps at random for between 1 and 5 seconds, then signals next thread and 
 *  goes back to blocking on condition_signal.
 */
void thd_worker (const int id, int &next_thd, std::default_random_engine &rand_e) {
    
    int wait_tm; // time to randomize

    thd_printer(id, "starting, waiting.");

    // repeat forever
    while(1) {
    
        // lock mutex
        mtx.lock();
        std::unique_lock<std::mutex> locker (mtx, std::adopt_lock);
        
        /* wait for condition signal
         * Upon condition signal, check if current thread is next
         * if yes continue, if not keep waiting
         * lambda function creates condition predicate */ 
        cond.wait(locker, [&]() { return id == next_thd; });

        thd_printer(id, "signal received, doing work ....");

        // generate pseudo-random value between 1 and 5 seconds
        wait_tm = 1 + rand_e() % 5; 
         
        // sleep to simulate work
        std::this_thread::sleep_for(std::chrono::seconds(wait_tm));

        thd_printer(id, "done with work, signal next thread");

        // if topmost thread, reset next_thd
        if(next_thd == NUM_THDS-1) next_thd = 0;
        else( ++next_thd); // otherwise, just increment 

        cond.notify_all();  // restart sequence
    }
}
```

## Main Function

The main function is built on the one in [Step_2](Step_2.md).

A point of note: It appears that thread0 will nearly always experiene spurious wake prior to the 3-second timer in main expiring. So, main initially sets the variable the threads use to keep track of order to a value outside of their possible range. When it is finally ready for the threads to start, it updates the variable after it asserts `cond.notify_all`.

```C++
int main () {

    int id = -1; // identifier to pass to Debug Printer

    std::thread threads[NUM_THDS]; /// create an array of NUM_THDS thread objects
    int next_thd = -1; // prevent a spurious wake from prematurely activating thread 0

    std::default_random_engine rand_e; /// create a pseudo-random engine
   
    std::cout << "main: starting all threads" << std::endl;

    // spawn NUM_THDS threads:
    for (int i=0; i<NUM_THDS; ++i) {
        
        /** populate the array of thread objects
         *  pass in: * their unique ID by value
         *           * an integer to keep track of thread order by reference
         *           * a shared psuedo-random number generator by reference */
        threads[i] = std::thread(thd_worker, i, std::ref(next_thd), std::ref(rand_e));
    }

    // wait for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    cond.notify_all();  // start sequence
    next_thd = 0;       // allow thread 0 to be activated

    // clean up
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
```

### Debug Print Function

While coding, I like to write debug functions that can easily be turned on and off. Here is an example:

```C++
// Globals
bool DEBUG = true;             // turn on debug messages


/** \brief DEBUG PRINTER function
 *
 *  \param id an integer that defines thread number (0 for main)
 *  \param msg a string containing message to be printed
 *
 * Prints to standard out
 * \warning NOT thread-safe; must be called within a thread-safe scope
 */
void DBG_PRNTR(int id, std::string msg) {
    
    std::cerr << "***\tDEBUG from" << id+1 << ": " 
              << msg << "\t***" << std::endl;
}


void thd_printer(int id, std::string msg) {
    //bool DEBUG = true;

    if(DEBUG) DBG_PRNTR(id, "made it to thd_printer");
    
...

}


void thd_worker (const int id, int &next_thd, std::default_random_engine &rand_e) {
    //bool DEBUG = true;
    
    if(DEBUG) DBG_PRNTR(id, next_thd);
    
}
```

These statements are peppered around my code in anticipation of adding other functionality. I have included additional copies of my cpp files without that code for easier reading. Eg.:

* [main.cpp](../src/Step_3/main.cpp) (with DEBUG code)
* [main_no_debug.cpp](../src/Step_3/main_no_debug.cpp) (identical, except without DEBUG code)


<br>

C++ Reference Material
---

Area to capture information for reference

### Condition Variable

std::condition_variable seems to be able to use std::lock_guard in the calling thread, but the receiving threads **must** use std::unique_lock. https://en.cppreference.com/w/cpp/thread/condition_variable

---

## Function Pointers

```C++
void HelloWorld() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    void(*function_name)();
    
    // does the same thing as this (without assignment):
    // auto function_name = &HelloWorld; // implicit &, so don't need
    
}
```


Also: 

```C++
int main() {
    typedef void(*function_name)();
    
    function_name = Helloworld;
}
```


