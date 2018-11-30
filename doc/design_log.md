# Design Log

## Preamble

The purpose of this design log is twofold:

1. I intend it to be a detailed log of the structure of my code, and the decisions I made while coding. I will point to Doxygen examples when relevant.
2. I intend it to be a demonstration of my problem-solving skills. To that end, I will capture each problem I encounter, and detail the steps I take to resolve it.

## Step 1

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl

### Prelims

#### Problem
Having difficulty parsing prompt. I took "LockGuard.h // header file only impl" in the prompt to indicate that I am to build a declaration for the class based off of a .h file that was provided. 

I see that no LockGuard.h was attached to the email. Perhaps I misunderstand what I am intended to do?

#### Attempt at Solution
Dive more in depth into the Mutex header files.

Need a better understanding of what lock_guard actually does. Brief googling leads to https://en.cppreference.com/w/cpp/thread/lock_guard Suspect I will want to call my class in a similar fashion. Perhaps use that code snippet as a template.

https://en.cppreference.com/w/cpp/header/mutex has this:

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

##### Deep dive

```C++
template <class Mutex> 
```

'meta programming' - programming what compiler does at runtime, rather than what program does at runtime

Reference: http://www.cplusplus.com/doc/oldtutorial/templates/

> The format for declaring function templates with type parameters is:
>
> ```
> template <class identifier> function_declaration;
> template <typename identifier> function_declaration;
> ```




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

