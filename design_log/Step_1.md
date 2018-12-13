# Step 1: Reimplement LockGuard

Requirements
---

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl

Prelims
---

#### Parsing Requirements

Initially had trouble parsing requirements. See [Reference in Appendix](Step_1_Appendix.md#initial-prompt) for details.

#### Environment

Performed all coding in a virtual machine running Ubuntu 17.10, 64-bit. To test build environment, created [hello world function](../src/Step_1/hello.cpp), and built with [makefile](../src/Step_1/makefile). Build successful.

#### Refamiliarization with C++

Needed to refamiliarize myself with C++ after spending large amount of time with C and Python. See [Reference in Appendix](Step_1_Appendix.md#c-reference-material) for details.



Method
---

### Research

Details about my research into lock_guard, mutex, and testing are available here: [Reference in Appendix](/Step_1_Appendix.md#what-does-this-actually-do)

### Coding

As this is a first step in a larger project, I intended to replicate already-working code. To that end, I copied `std::lock_guard` into a new header file, fixed references to an STD object that was out of namespace, and encapsulated it with my own namespace. To test, I took generic multithread example code that uses std::lock_guard and modified it to use my namespace and class. See [Reference in Appendix](Step_1_Appendix.md#coding) for details.

## Test conditions

* Pass: The test code performs the same with my namespace and class, `chal::LockGuard`, as it would with `std::lock_guard`.
* Fail: Any other result.

Results
---

The test code passed. It performed the same when `std::lock_guard` was referenced, and when `chal::LockGuard` was referenced. It produced this output, which matched expectations:

```bash
armaan@ubuntuVM:Step_1$ ./Lock.out 
thread #7
thread #8
thread #9
thread #6
thread #10
thread #5
thread #4
thread #3
thread #2
thread #1
```

To duplicate
---

In [/src/Step_1/ directory](../src/Step_1) run `make`. Executable will build as *Lock.out*. To run executable, run `./Lock.out`.

Files
---

* [LockGuard.h](../src/Step_1/LockGuard.h)
* [main.cpp](../src/Step_1/main.cpp)
