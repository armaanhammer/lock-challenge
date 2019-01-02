# Step 2: Focus in on threading use case

To duplicate
---

In [/src/Step_2/](../src/Step_2) run `make`. Executable will build as *Lock.out*. To run executable, run `./Lock.out`.


Files
---

* [LockGuard.h](../src/Step_2/LockGuard.h)
* [main.cpp](../src/Step_2/main.cpp)


Results
---

It produced this output, which matched expectations:

```bash
armaan@ubuntuVM:Step_2$ ./Lock.out 
thread #3
thread #2
thread #1

```


Test conditions
---

* Pass: The test code performs the same with my namespace and class, `chal::LockGuard`, as it would with `std::lock_guard`.
* Fail: Any other result.


Method 
---

### Coding

The example I used from Step 1 is already threadable with 10 threads called, so I intend to take current LockGuard testbench and change it to 3 threads. See [Reference in Appendix](Step_2_Appendix.md#coding) for details.


Requirements
---

> 2) Implement c++11 threading example
>
>     2-3 threads running


Prelims
---

### Parsing Requirements

Was initially uncertain about parsing prompt. Read ahead to Step 3 to illuminate requirments. [Discussion in Appendix](Step_2_Appendix.md#intial-prompt)

### Environment

Environment remains the same as the previous step.
