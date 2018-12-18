# Step 4: Protect a Common Resource

Requirements
---

>  4) Use your implementation of c++11 lock_guard (NOT std::lock_guard)
>     to protect a common resource in your threading example from #3 and #4

<br>

Prelims
---

### Environment

Environment remains the same as the previous steps.


<br>

Method
---

### Coding

I chose `cout` as my common resoure to protect. To accomplish this, I only need to lock a mutex and call my `chal::LockGuard` function from within the `thd_printer` again.

I debated about this for a bit. See [Reference in Appendix](Step_4_Appendix.md#coding) for details. In the end, I decided to enable DEBUG printing inside of the `chal::LockGuard` class to demonstrate that locking and unlocking are occuring with each write to standard out from the threads.

### Doxygen

Considerations for creating documentation remain the same as for Step 3: [Reference in Step 3 Appendix](Step_3_Appendix.md#doxygen-creation)

### Test conditions

* Pass: 
  * Duplicate behavior from Step 3.
  * Avoid anomalous thread behavior, including but not limted to, deadlock and data races.
  * Demonstrate that the resource is protected.
* Fail: Any other result.


<br>

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

To duplicate
---

1. Enure that the bool value for `DEBUG` inside of LockGuard.h is set to `true`.
1. In [/src/Step_4/ directory](../src/Step_4) run `make`. Executable will build as *Lock.out*. 
1. To run executable and pipe standard error to a file, run `./Lock.out 2> error_log.txt`.

<br>

Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 4 main page](https://armaanhammer.github.io/lock-challenge/Step_4/html/index.html) | | |
| --- | --- | --- |
| LockGuard.h | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/LockGuard_8h.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/LockGuard_8h_source.html) |
| main.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/main_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/main_8cpp_source.html) |
| main_no_debug.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_4/html/main__no__debug_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_4/html/main__no__debug_8cpp_source.html) |

<br>


Files
---

* [LockGuard.h](../src/Step_4/LockGuard.h)
* [main.cpp](../src/Step_4/main.cpp)
* [main_no_debug.cpp](../src/Step_4/main_no_debug.cpp)
* [error_log.txt](../src/Step_4/error_log.txt)