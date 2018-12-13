# Step 2: Focus in on threading use case

Requirements
---

> 2) Implement c++11 threading example
>
>     2-3 threads running

<br>


Prelims
---

#### Parsing Requirements

Was initially uncertain about parsing prompt. Read ahead to Step 3 to illuminate requirments. [Discussion in Appendix](Step_2_Appendix.md#intial-prompt)

#### Environment

Environment remains the same as the previous step.

<br>


Method 
---

### Coding

The example I used from Step 1 is already threadable with 10 threads called, so I intend to take current LockGuard testbench and change it to 3 threads. See [Reference in Appendix](Step_2_Appendix.md#coding) for details.

## Test conditions

* Pass: The test code performs the same with my namespace and class, `chal::LockGuard`, as it would with `std::lock_guard`.
* Fail: Any other result.


<br>

Method
---

### Coding

As this is a first step in a larger project, I intended to replicate already-working code. To that end, I copied `std::lock_guard` into a new header file, fixed references to an STD object that was out of namespace, and encapsulated it with my own namespace. To test, I took generic multithread example code that uses std::lock_guard and modified it to use my namespace and class. See [Reference in Appendix](Step_2_Appendix.md#coding) for details.

## Test conditions

* Pass: The test code performs the same as in Step 1, but with 3 threads.
* Fail: Any other result.


<br>

Results
---

It produced this output, which matched expectations:

```bash
armaan@ubuntuVM:Step_2$ ./Lock.out 
thread #3
thread #2
thread #1

```

To duplicate
---

In [/src/Step_2/](../src/Step_2) run `make`. Executable will build as *Lock.out*. To run executable, run `./Lock.out`.


<br>

Files
---

* [LockGuard.h](../src/Step_2/LockGuard.h)
* [main.cpp](../src/Step_2/main.cpp)

