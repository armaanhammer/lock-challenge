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

Initially had trouble parsing requirements. [Reference in Appendix](Step_3_Appendix.md#initial-prompt)

### Environment

Environment remains the same as the previous steps.

### Refamiliarization with C++

Continue needing to refamiliarize myself with C++ after spending large amount of time with C and Python. See [Reference in Appendix](Step_3_Appendix.md#c-reference-material) for details.


<br>

Method
---

### Research

Details about my research into ________________ are available here: [Reference in Appendix](Step_3_Appendix.md#research)

### Coding

I intend to ________________ See [Reference in Appendix](Step_1_Appendix.md#coding) for details.

### Test conditions

* Pass: .
* Fail: Any other result.

### Doxygen

Considerations for creating documentation are listed in: [Reference in Appendix](Step_3_Appendix.md#doxygen-creation)


<br>

Results
---


To duplicate
---

In [/src/Step_3/ directory](../src/Step_3) run `make`. Executable will build as *Lock.out*. To run executable, run `./Lock.out`.

<br>

Files
---

* [LockGuard.h](../src/Step_3/LockGuard.h)
* [main.cpp](../src/Step_3/main.cpp)
