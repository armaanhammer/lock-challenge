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

## Parsing Prompt:

Uncertain what "solution from #3" in `Use c++11 std::condition_var in solution from #3` refers to. Going to assume it is a typo from a previous version of the same prompt, and instead focus on the std function.

Also uncertain what line with //gimmie is talking about. My assumption based on the example output is that all three threads need to loop sequentially. I am uncertain from the prompt whether they should be started and joined repeatedly in main, or if they should run forever by themselves. To attempt to parse this, will write code for a bit and see which is more reasonable.


<br>

Research
---

### Sizing

Want variable sizing (at compile time) - add more or less threads at will and have program perform the same. Feel this is good coding practice, and can potentially reduce complexity.

### How to guarantee ordered thread execution

Thinking of two ways to guarantee ordered thread execution:
* One mutex per thread-to-thread handoff
  * Each possible transition (eg. 1-2 , 2-3 , 3-1) has its own mutex that the 'receiving' thread blocks on.
  * main() is responsible for assigning mutexes to correct threads (eg. thread 3 must be assigned mutex 1 as mutex to unlock)
* One mutex total, with an int passed by reference to maintain count of which thread is next. 
  * each thread must have logic to increment in (eg. thread 3 must 'increment' int to 1)
  
Decided on single mutex and shared int by reference. Reasoning being: each thread already needs to know its ID to print it out. Might as well capitalize on that.

### Storage of thread and objects

* Chosing to continue storing thread objects either in arrays or a vector for ease of joining later.
  * Ran into question about whether to use array vs. vector. 
    * Pros for array:
      * easy
      * quick
    * Cons for array:
      * harder to count size of
      * less expandible
    * Pros for vector:
      * far more expandable
      * possible to easily size
    * Cons for vector:
      * needless complexity
      * slower
      
Decided on array. May update later.

### Indexing

Decided on 0-indexing because easier to code. Will +1 for cout statements so appears as 1-indexing to comply with display output requirements.


### Doxygen Creation

Considering how display STL library references in code. `BUILTIN_STL_SUPPORT    = YES/NO` is relevant line.

`EXTRACT_ALL            = NO`

and

`EXTRACT_PRIVATE        = NO`

also under consideration.






<br>

Coding
---

### Threading

Initial idea:

    /* PSUEDO CODE
     * block on condition_variable and if id != &next_thd
     * sleep for 1-5 secs
     * lock mutex
     * lock_guard(mutex)
     * increment next_thd
     *   if this is topmost thread, reset next_thd
     *   else increment next_thd
     * call thread_printer function, passing in mutex
     *   in thread printer function:
     *     capture mutex from calling function
     *     print
     *
     */ 
     
### chal::LockGuard vs. std::unique_lock

Realized that `std::condition_variable` **must** use `std::unique_lock` to wake on notify. This constrains my code, preventing me from using `chal::LockGuard` unless I make significant modificantions. Reading ahead to [Step 4](Step_4.md), I see that I can use it there to protect a resource. Not going to use it for this step.

### Synchronizing Threads





<br>

C++ Reference Material
---

Area to capture information for reference

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


