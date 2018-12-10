# Step 3 Appendix

## Initial Prompt

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

---


## Parsing Prompt:

Uncertain what "solution from #3" in `Use c++11 std::condition_var in solution from #3` refers to. Going to assume it is a typo from a previous version of the same prompt, and instead focus on the std function.

Also uncertain what line with //gimmie is talking about. My assumption based on the example output is that all three threads need to loop sequentially. I am uncertain from the prompt whether they should be started and joined repeatedly in main, or if they should run forever by themselves. To attempt to parse this, will write code for a bit and see which is more reasonable.

---

### Condition Variable

std::condition_variable seems to be able to use std::lock_guard in the calling thread, but the receiving threads **must** use std::unique_lock. https://en.cppreference.com/w/cpp/thread/condition_variable

---

## Function Pointers

```C++
void HelloWorld() {
    std::cout << "Hello World!" << std::endl;
}

int main() {
    void(*function_name)();
    
    // does the same thing as this (without assignment):
    // auto function_name = &HelloWorld; // implicit &, so don't need
    
}
```


Also: 

```C++
int main() {
    typedef void(*function_name)();
    
    function_name = Helloworld;
}
```

## Doxygen Creation

Considering how display STL library references in code. `BUILTIN_STL_SUPPORT    = YES/NO` is relevant line.
