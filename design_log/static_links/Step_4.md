# Step 4: Protect a Common Resource

To duplicate
---

1. Enure that the bool value for `DEBUG` inside of LockGuard.h is set to `true`.
1. In [Step_4/ directory](https://github.com/armaanhammer/lock-challenge/tree/master/src/Step_4) run `make`. Executable will build as *Lock.out*. 
1. To run executable and pipe standard error to a file, run `./Lock.out 2> error_log.txt`.


Files
---

* [LockGuard.h](https://github.com/armaanhammer/lock-challenge/tree/master/src/Step_4/LockGuard.h)
* [main.cpp](https://github.com/armaanhammer/lock-challenge/tree/master/src/Step_4/main.cpp) (with DEBUG code)
* [main_no_debug.cpp](https://github.com/armaanhammer/lock-challenge/tree/master/src/Step_4/main_no_debug.cpp) (identical, except without DEBUG code)
* [error_log.txt](https://github.com/armaanhammer/lock-challenge/tree/master/src/Step_4/error_log.txt) 


Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 4 main page](https://armaanhammer.github.io/lock-challenge/Step_4/html/index.html) | | |
| --- | --- | --- |
| LockGuard.h | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/LockGuard_8h.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/LockGuard_8h_source.html) |
| main.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/main_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/main_8cpp_source.html) |
| main_no_debug.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/main__no__debug_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/main__no__debug_8cpp_source.html) |


Results
---

It produced this output to the terminal when combining standard out and standard error:

```bash
armaan@ubuntuVM:Step_4$ ./Lock.out
main: starting all threads
LockGuard adopted
thread3: starting, waiting.
LockGuard unlocked
LockGuard adopted
thread2: starting, waiting.
LockGuard unlocked
LockGuard adopted
thread1: starting, waiting.
LockGuard unlocked
LockGuard adopted
thread1: signal received, doing work ....
LockGuard unlocked
LockGuard adopted
thread1: done with work, signal next thread
LockGuard unlocked
LockGuard adopted
thread2: signal received, doing work ....
LockGuard unlocked
LockGuard adopted
thread2: done with work, signal next thread
LockGuard unlocked
LockGuard adopted
thread3: signal received, doing work ....
LockGuard unlocked
LockGuard adopted
thread3: done with work, signal next thread
LockGuard unlocked
LockGuard adopted
thread1: signal received, doing work ....
LockGuard unlocked
^C
```

And this output when only printing standard out:

```bash
armaan@ubuntuVM:Step_4$ ./Lock.out 2> error_log.txt
main: starting all threads
thread3: starting, waiting.
thread2: starting, waiting.
thread1: starting, waiting.
thread1: signal received, doing work ....
thread1: done with work, signal next thread
thread2: signal received, doing work ....
thread2: done with work, signal next thread
thread3: signal received, doing work ....
thread3: done with work, signal next thread
thread1: signal received, doing work ....
^C
```

And this output when only printing standard error:

```bash
armaan@ubuntuVM:Step_4$ cat error_log.txt
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
LockGuard adopted
LockGuard unlocked
armaan@ubuntu373VM:Step_4$ 
```

These outputs match expectations.


Test conditions
---

* Pass: 
  * Duplicate behavior from Step 3.
  * Avoid anomalous thread behavior, including but not limted to, deadlock and data races.
  * Demonstrate that the resource is protected.
* Fail: Any other result.


Method
---

### Coding

I chose `cout` as my common resoure to protect. To accomplish this, I only need to lock a mutex and call my `chal::LockGuard` function from within the `thd_printer` again.

I debated about this for a bit. See [Reference in Appendix](https://github.com/armaanhammer/lock-challenge/tree/master/design_log/Step_4_Appendix.md#coding) for details. In the end, I decided to enable DEBUG printing inside of the `chal::LockGuard` class to demonstrate that locking and unlocking are occuring with each write to standard out from the threads.

### Doxygen

Considerations for creating documentation remain the same as for Step 3: [Reference in Step 3 Appendix](https://github.com/armaanhammer/lock-challenge/tree/master/design_log/Step_3_Appendix.md#doxygen-creation)


Requirements
---

>  4) Use your implementation of c++11 lock_guard (NOT std::lock_guard)
>     to protect a common resource in your threading example from #3 and #4


Prelims
---

### Environment

Environment remains the same as the previous steps.
