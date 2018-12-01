

# Step 1

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl

lock_guard is a function that implements a programming idiom called **Resource Acquisition is Initialization** or **RAII**. In this idiom, resource acquisition is tied to object lifetime. lock_guard ties a resource (mutex) to an object (the calling function). The mutex is released upon execution leaving the scope of the function that called lock_guard. This provides a safeguard against an exception in the calling function preventing it from releasing the lock directly.

## Prelims

### Problem
Having difficulty parsing prompt. I took "LockGuard.h // header file only impl" in the prompt to indicate that I am to build a declaration for the class based off of a .h file that was provided.

I see that no LockGuard.h was attached to the email. Perhaps I misunderstand what I am intended to do?

### Attempt at Solution
Dive more in depth into the Mutex header files.

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

#### Deep dive

#### Mutex

```C++
template <class Mutex>
```

The template directive is 'meta programming'; it programs what the compiler does at compile time, rather than what program does at runtime. I find the `template <class identifier> function_declaration;` somewhat confusing, and prefer to substitute `template <typename identifier> function_declaration;` which is synonymous.

##### [Reference in Appendix](Step_1_Appendix.md#Mutex)

---

#### Operator Overload

```C++
lock_guard& operator=(lock_guard const&) = delete;
```

Appears to be operator overloading the equals sign. Need to parse this better.


##### [Reference in Appendix](Step_1_Appendix.md#Operator-Overload)

---


##### remaining code to work through:

```C++
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

---

### Trying a new approach - look at the member functions

http://www.cplusplus.com/reference/mutex/lock_guard/


