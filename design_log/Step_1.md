# Step 1: Reimplement LockGuard

## Requirements

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl

## Prelims

Initially had trouble parsing requirements. [Reference in Appendix](Step_1_Appendix.md#Initial-Prompt)

## Method

As a first step, I intended to replicate already-working code. To that end, I copied `std::lock_guard` into a new header file, fixed references to an STD object that was out of namespace, and encapsulated it with my own namespace. To test, I took generic multithread example code that uses std::lock_guard and modified it to use my namespace and class.

## Test conditions

* Pass: The test code performs the same with my namespace and class, `chal::LockGuard`, as it would with `std::lock_guard`.
* Fail: Any other result.

## Results

The test code passed. It performed the same when `std::lock_guard` was referenced, and when `chal::LockGuard` was referenced. It produced this output, which matched expectations:

> ```bash
> armaan@ubuntuVM:Step_1$ ./Lock.out 
> thread #7
> thread #8
> thread #9
> thread #6
> thread #10
> thread #5
> thread #4
> thread #3
> thread #2
> thread #1
> ```
