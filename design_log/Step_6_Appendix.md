# Step 6 Appendix

## CMAKE

### Parsing

Parsing the [included CMakeLists_1.txt](../prompt/CMakeLists_1.txt):

```
cmake_minimum_required(VERSION 3.8)

project (test-memory-pool
        VERSION 1.0.0
        DESCRIPTION "Test Memory Pool")

include_directories(.)
link_directories(../ ./)

set(CMAKE_CXX_FLAGS "-Wall -std=c99")

# create executable
add_executable (test test-memory-pool.c memory_pool.c)
set_property(TARGET test PROPERTY C_STANDARD 99)
```

Ok, so this is going to be straight C, rather than C++. Going to copy [memory_pool.c prompt](../prompt/memory_pool.c) and [memory_pool.h prompt](../prompt/memory_pool.h) to this project's src.

Also going to add debug symbols. Trying adding this line: `set(CMAKE_BUILD_TYPE Debug)`, and will also add flag `-g` to this line: `set(CMAKE_CXX_FLAGS "-Wall -std=c99")` to pass in explicitly. 


<br>


Analysis
---

### Initial

#### CMAKE

The presence of `test-memory-pool.` in the create executable directives indicates that I am to create a testbench on my own.

#### memory_pool.h

I am given a list of objectives:

```
 * Objective: speed operations of malloc/free and adapt idiomatically and separate memory
             management from other data storage patterns such as linked lists, stacks,
             double buffering
   Limitations: Fixed sized memory blocks. Due to the O(1) requirement only fixed sized
                memory allocation can be performed. Memory fragmentation and
                collection/collating operations are not desired due to performance demands
  Support O(1) operation in acquire and release operations
  Strategy:
    stack object to manage memory blocks
       acquire = pop_front  (acquire block off the front/top of stack)
       release = push_back  (release block by putting on back/bottom of stack)
```

#### memory_pool.c





  
