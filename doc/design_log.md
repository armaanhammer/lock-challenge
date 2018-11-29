# Design Log

## Step 1

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard

>      LockGuard.h          // header file only impl

### Prelims

Need a better understanding of what lock_guard actually does. Brief googling leads to https://en.cppreference.com/w/cpp/thread/lock_guard Suspect I will want to call my class in a similar fashion. Perhaps use that code snippet as a template.



