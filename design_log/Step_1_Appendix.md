# Step 1 Appendix

The purpose of this document is to capture ancillary research I have done that only loosely applies to the objectives I am working toward.

<br>

Initial Prompt
---

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl

### Parsing Prompt
Having difficulty parsing prompt. I took "LockGuard.h // header file only impl" in the prompt to indicate that I am to build a declaration for the class based off of a .h file that was provided.

I see that no LockGuard.h was attached to the email. Perhaps I misunderstand what I am intended to do?

### Additional Prompt

Upon asking for clarification, was provided this:

> You need to create LockGuard.h. I did not attach it. You need to re-create the functionality of std::lock_guard.  You can put all the code in a header file. It’s a common technique for STL and other utilities and is often referred to as “header only” so the user of the utility knows they can just include the header and not have to link a library to their project.
>
> You will need to fully implement the functions. In the header file.
>
> LockGuard.h
>
> Set namespace to avoid naming collisions with std::lock_guard. std:: being a name space.
>
> class LockGuard {
>
>   … implementation …
>
> }



<br>

What does this actually do?
---

### Diving into std::lock_guard and std::mutex

Need a better understanding of what lock_guard actually does. Brief googling leads to a template for calling the std class here: https://en.cppreference.com/w/cpp/thread/lock_guard Suspect I will want to call my class in a similar fashion. Pasting code here for reference:

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

Now, to look at the headers.
https://en.cppreference.com/w/cpp/header/mutex has these:

```C++
class mutex {
 public:
    constexpr mutex() noexcept;
    ~mutex();
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    void lock();
    bool try_lock();
    void unlock();
    typedef /*implementation-defined*/ native_handle_type;
    native_handle_type native_handle();
};
```

and

```C++
template <class Mutex>
class lock_guard {
 public:
    typedef Mutex mutex_type;
    explicit lock_guard(mutex_type& m);
    lock_guard(mutex_type& m, adopt_lock_t);
    ~lock_guard();
    lock_guard(lock_guard const&) = delete;
    lock_guard& operator=(lock_guard const&) = delete;
 private:
    mutex_type& pm; // exposition only
};
```

### Diving deeper - looking at the member functions

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

### Testbench
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

<br>

Coding
---

### LockGuard

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

### LockGuard Testbench

For the testbench, I copied the generic example code and changed the reference for std::lock_guard to chal::LockGuard .

```C++
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::adopt_lock
#include "LockGuard.h"    // chal::LockGuard

std::mutex mtx;           // mutex for critical section

void print_thread_id (int id) {
    mtx.lock();
    
    // Swap next two lines to test chal::LockGuard against std::lock_guard
    //std::lock_guard<std::mutex> lck (mtx, std::adopt_lock);
    chal::LockGuard<std::mutex> lck (mtx, std::adopt_lock);

    std::cout << "thread #" << id << '\n';
}

int main ()
{
    std::thread threads[10]; // creates an array of 10 thread objects
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_thread_id,i+1);

    // for each element th in array threads, join th
    // use auto& to guarantee sharing variable by reference 
    for (auto& th : threads) th.join();

    return 0;
}

```



<br>

C++ Reference Material
---

Area to capture information for reference

### Lock_Guard in abstract

lock_guard is a function that implements a programming idiom called **Resource Acquisition is Initialization** or **RAII**. In this idiom, resource acquisition is tied to object lifetime. lock_guard ties a resource (mutex) to an object (the calling function). The mutex is released upon execution leaving the scope of the function that called lock_guard. This provides a safeguard against an exception in the calling function preventing it from releasing the lock directly.

###### Reference:

https://www.youtube.com/watch?v=ojOUIg13g3I

#### Template
```C++
template <class Mutex>
```
The template directive is 'meta programming'; it programs what the compiler does at compile time, rather than what program does at runtime. I find the `template <class identifier> function_declaration;` somewhat confusing, and prefer to substitute `template <typename identifier> function_declaration;` which is synonymous.

###### Reference:
http://www.cplusplus.com/doc/oldtutorial/templates/


#### Mutex

https://www.youtube.com/watch?v=I-hZkUa9mIs  <<< Interesting that this video says calling the mutex's own lock&unlock function is _not_ recommended. This seems to imply that in general, it is best to use a lock_guard function. Also notes that lock_guard does not release the lock until out of scope, unlike unique_lock, which releases the lock upon `locker.unlock();`. This provides a constraint on my code later.

https://www.youtube.com/watch?v=LL8wkskDlbs  <<< Initial video of C++ Threading series. Realizing it would be useful to watch them all to brush up. Diverting to doing that now.

Points of note:

> Once a parent thread calls a child thread, it is important to encapsulate any further work the parent thread does before re-joining the child thread in a try, catch block to prevent an exception in the parent from destroying the child before it is done. Ex:
>
> ```C++
> int main(){
>     std::thread t1(function_1);  // t1 starts running
>
>     try {
>           do_something_dangerous();
>     } catch (...) {
>           t1.join();
>           throw;
>
>     ti.join();
>     return 0;
> }
> ```



#### Typedef
```C++
    typedef Mutex mutex_type;
```
The typedef-names are aliases for existing types, and are not declarations of new types. Eg.
> ```C++
> // simple typedef
> typedef unsigned long ulong;
> ```




#### Operator Overload
```C++
lock_guard& operator=(lock_guard const&) = delete;
```
Appears to be operator overloading the equals sign. Need to parse this better.

###### Note:
"Passing by const reference to avoid copying". Any function that doesn't modify the class should be const.
> ```C++
> Vector2 Add(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
> ```

The operator overload analog:
> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
> ```

An alternative approach is:

> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
>
> Vector2 Add(const Vector2& other) const {
>     return *this + other;  // deref because 'this' is const pntr in current scope
> }
> ```

Yet another alternative approach is:

> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
>
> Vector2 Add(const Vector2& other) const {
>     return operator+(other);  // address operator+ like a function
> }
> ```


###### Overloading the << operator
Overloading the << operator to include Vector2 containing two floats
> ```C++
> std::ostream& operator<<(std::ostream& stream, const Vector2& other) {
>     stream << other.x << ", " << other.y;
>     return stream;
> }
> ```

###### Reference:

https://www.youtube.com/watch?v=PgGhEovFhd0

https://www.youtube.com/watch?v=mS9755gF66w  <<< Says best practice: only use when usage is intuitive. Avoid if ppl would need to go to your function to understand what it does.


#### Range based for loop (new to C++11)

```C++
    // for each element th in array threads, join th
    // use auto& to guarantee sharing variable by reference 
    for (auto& th : threads) th.join();
```

syntax:

```C++
for ( element : collection ) {
      //do something
}
```

###### Reference:

https://stackoverflow.com/questions/29859796/c-auto-vs-auto

https://www.youtube.com/watch?v=2yR8dcKT-Ck

<br>

Questions to resolve, not relevant to project
---

What does this do? Says it is a constructor definition. Occurs inside of a struct.
> ```C++
> Vector2(float x, float y)
>     : x(x), y(y) {}
> ```

###### Reference:

https://www.youtube.com/watch?v=mS9755gF66w


