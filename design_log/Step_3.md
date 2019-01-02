# Step 3: Loop threads with condition_var

Requirements
---

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

<br>

Prelims
---

### Parsing Requirements

Initially had trouble parsing requirements. [Reference in Appendix](Step_3_Appendix.md#parsing-prompt)

### Environment

<<<<<<< HEAD
Files and Documentation
---
=======
Environment remains the same as the previous steps.
>>>>>>> parent of 22bda4f... Reordered sections in Step_3.md

### Refamiliarization with C++

Continue needing to refamiliarize myself with C++ after spending large amount of time with C and Python. See [Reference in Appendix](Step_3_Appendix.md#c-reference-material) for details.

<<<<<<< HEAD
[Step 3 Doxygen main page](https://armaanhammer.github.io/lock-challenge/Step_3/html/index.html)

| File on GitHub | Code in Doxygen | Documentation in Doxygen |
| --- | --- | --- |
| [LockGuard.h](../src/Step_3/LockGuard.h) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/LockGuard_8h_source.html) | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/LockGuard_8h.html) |
| [main.cpp](../src/Step_3/main.cpp) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/main_8cpp_source.html) | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/main_8cpp.html) |
| [main_no_debug.cpp](../src/Step_3/main_no_debug.cpp) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/main__no__debug_8cpp_source.html) | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/main__no__debug_8cpp.html) |
=======
>>>>>>> parent of 22bda4f... Reordered sections in Step_3.md

<br>

Method
---

### Research

Details about my research into threading and other topics are available here: [Reference in Appendix](Step_3_Appendix.md#research)

### Coding

I wrote three functions to do work, and one overloaded function to facilitate debug. See [Reference in Appendix](Step_1_Appendix.md#coding) for details. In brief, they are: 
* A [general-purpose print function](Step_3_Appendix.md#thread-print-function) that the threads call from a thread-safe state
* A [thread worker function](Step_3_Appendix.md#thread-worker-function) that listens for a signal, verifies whether it is its turn or not, and updates the other threads on whose turn it is.
* A [main function](Step_3_Appendix.md#main-function) that initializes everything up and sets it in motion.
* A [debug print function](Step_3_Appendix.md#debug-print-function) called from many places in the code only if a bool controlling it is set to *true*.

### Doxygen

Considerations for creating documentation are discussed here: [Reference in Appendix](Step_3_Appendix.md#doxygen-creation)

### Test conditions

* Pass: 
  * Produce output consistent with *example output* in prompt
  * Avoid anomalous thread behavior, including but not limted to, deadlock and data races.
* Fail: Any other result.


<br>

Results
---
It produced this output, which matched expectations:

```bash
armaan@ubuntuVM:Step_3$ ./Lock.out 
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
thread1: done with work, signal next thread
thread2: signal received, doing work ....
^C
armaan@ubuntuVM:Step_3$ 
```

To duplicate
---

In [/src/Step_3/ directory](../src/Step_3) run `make`. Executable will build as *Lock.out*. To run executable, run `./Lock.out`.

<br>

Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 3 main page](https://armaanhammer.github.io/lock-challenge/Step_3/html/index.html) | | |
| --- | --- | --- |
| LockGuard.h | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/LockGuard_8h.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/LockGuard_8h_source.html) |
| main.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/main_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/main_8cpp_source.html) |
| main_no_debug.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_3/html/main__no__debug_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_3/html/main__no__debug_8cpp_source.html) |

<br>

Files
---

* [LockGuard.h](../src/Step_3/LockGuard.h)
* [main.cpp](../src/Step_3/main.cpp)
* [main_no_debug.cpp](../src/Step_3/main_no_debug.cpp)
