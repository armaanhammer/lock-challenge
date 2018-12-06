# Step 1 - Reimplement LockGuard

lock_guard is a class that implements a programming idiom called **Resource Acquisition is Initialization** or **RAII**. In this idiom, resource acquisition is tied to object lifetime. lock_guard ties a resource (mutex) to an object (the calling function). The mutex is released upon execution leaving the scope of the function that called lock_guard. This provides a safeguard against an exception in the calling function preventing it from releasing the lock directly.

## Prelims

Initially had trouble parsing requirements. [Reference in Appendix](Step_1_Appendix.md#Initial-Prompt)

---

## Pass/Fail Conditions

I plan to use a Test-Based approach for this each step of this coding challenge. In this step, the test will be whether my new LockGuard class duplicates the functionality of the std::lock_guard class. Duplicate behavior will pass. Behaviour that deviates will fail.

## Method

I will keep my new class as identical to std::lock_guard as possible, and I will test it using one (or more) generic example usages of std::lock_guard .

## Source material

### class

I had very little luck finding fully implemented functions on the web, so I spun up an Ubuntu VM and searched around. I found it in `/usr/include/c++/5` under Ubuntu Desktop 17.10:

```C++
/** @brief A movable scoped lock type.
 *
 * A unique_lock controls mutex ownership within a scope. Ownership of the
 * mutex can be delayed until after construction and can be transferred
 * to another unique_lock by move construction or move assignment. If a
 * mutex lock is owned when the destructor runs ownership will be released.
 */
template<typename _Mutex>
  class lock_guard
  {
  public:
    typedef _Mutex mutex_type;

    explicit lock_guard(mutex_type& __m) : _M_device(__m)
    { _M_device.lock(); }

    lock_guard(mutex_type& __m, adopt_lock_t) : _M_device(__m)
    { } // calling thread owns mutex

    ~lock_guard()
    { _M_device.unlock(); }

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

  private:
    mutex_type&  _M_device;
  };
  ```

## testbench
I found a template for calling the std class here: https://en.cppreference.com/w/cpp/thread/lock_guard which I will use for initial testing for this step:

```C++
int g_i = 0;
std::mutex g_i_mutex;  // protects g_i

void safe_increment()
{
    std::lock_guard<std::mutex> lock(g_i_mutex);
    ++g_i;

    std::cout << std::this_thread::get_id() << ": " << g_i << '\n';

    // g_i_mutex is automatically released when lock
    // goes out of scope
}

int main()
{
    std::cout << "main: " << g_i << '\n';

    std::thread t1(safe_increment);
    std::thread t2(safe_increment);

    t1.join();
    t2.join();

    std::cout << "main: " << g_i << '\n';
}
```
---

## My code

## LockGuard

I copied the std::lock_guard class, added an std:: namespace reference to the reference that needed it, and encapsulated the new class in a new namespace "chal" for challenge:

```C++
namespace chal { // challenge namespace

    /** @brief A movable scoped lock type.
     *
     * This class has been kept as identical to std::lock_guard as possible
     *
     * A unique_lock controls mutex ownership within a scope. Ownership of the
     * mutex can be delayed until after construction and can be transferred
     * to another unique_lock by move construction or move assignment. If a
     * mutex lock is owned when the destructor runs ownership will be released.
     */
    template<typename _Mutex>  // returns type determined by calling function
    class LockGuard
    {
        public:
            typedef _Mutex mutex_type;

            explicit LockGuard(mutex_type& __m) : _M_device(__m)  // no implicit constructor
                { _M_device.lock(); }

            LockGuard(mutex_type& __m, std::adopt_lock_t) : _M_device(__m)
                { } // calling thread owns mutex

            ~LockGuard()
                { _M_device.unlock(); }

            // generate compile error if copy attempted
            // (supposed to be un-copyable)
            LockGuard(const LockGuard&) = delete;  // copy constructor
            LockGuard& operator=(const LockGuard&) = delete;  // copy assignment operator

        private:
            mutex_type&  _M_device;
    };
}
```

## LockGuard Testbench

For the testbench, I copied the generic example code and changed the reference for std::lock_guard to chal::LockGuard .

```C++
// constructing lock_guard with adopt_lock

//NOTE: needs to be compiled with flag -pthread.

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
```
---

## Results:

The example code performed the same when std::lock_guard was referenced, and when chal::LockGuard was referenced, so the test succeeded.
